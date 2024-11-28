/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#define USING_LOG_PREFIX SERVER

#include "observer/table_load/ob_table_load_redef_table.h"
#include "observer/table_load/ob_table_load_table_ctx.h"
#include "observer/table_load/ob_table_load_struct.h"
#include "share/ob_common_rpc_proxy.h"
#include "sql/engine/cmd/ob_ddl_executor_util.h"
#include "storage/ddl/ob_ddl_server_client.h"

namespace oceanbase
{
using namespace common;
using namespace sql;
using namespace obrpc;
namespace observer
{

int ObTableLoadRedefTable::start(const ObTableLoadRedefTableStartArg &arg,
                                 ObTableLoadRedefTableStartRes &res, ObSQLSessionInfo &session_info)
{
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!arg.is_valid())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid args", KR(ret), K(arg));
  } else if (session_info.get_ddl_info().is_mview_complete_refresh()) {
    ObExecContext *exec_ctx = session_info.get_cur_exec_ctx();
    const ObPhysicalPlanCtx *plan_ctx = nullptr;
    const ObPhysicalPlan *plan = nullptr;
    if (OB_ISNULL(exec_ctx)
        || OB_ISNULL(plan_ctx = exec_ctx->get_physical_plan_ctx())
        || OB_ISNULL(plan = plan_ctx->get_phy_plan())) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("unexpected null physical plan (ctx)", KR(ret), KP(plan_ctx), KP(plan));
    } else {
      res.task_id_ = plan->get_ddl_task_id();
      share::ObDDLTaskStatus status = share::ObDDLTaskStatus::PREPARE;
      if (OB_FAIL(ObDDLUtil::get_data_information(arg.tenant_id_,
          res.task_id_,
          res.data_format_version_,
          res.snapshot_version_,
          status,
          res.dest_table_id_,
          res.schema_version_,
          res.is_no_logging_))) {
        LOG_WARN("fail to get ddl task info", KR(ret), K(arg));
      }
    }
  } else {
    const int64_t origin_timeout_ts = THIS_WORKER.get_timeout_ts();
    ObCreateHiddenTableArg create_table_arg;
    ObCreateHiddenTableRes create_table_res;
    DEBUG_SYNC(BEFORE_CREATE_HIDDEN_TABLE_IN_LOAD);
    uint64_t tenant_id = arg.tenant_id_;
    const bool need_reorder_column_id = false;
    const share::ObDDLType ddl_type = arg.is_load_data_ ? share::DDL_DIRECT_LOAD : share::DDL_DIRECT_LOAD_INSERT;
    if (OB_FAIL(create_table_arg.init(tenant_id, tenant_id, tenant_id, arg.table_id_,
                                      THIS_WORKER.get_group_id(), session_info.get_sessid_for_table(),
                                      arg.parallelism_, ddl_type, session_info.get_sql_mode(),
                                      session_info.get_tz_info_wrap().get_tz_info_offset(),
                                      session_info.get_local_nls_date_format(),
                                      session_info.get_local_nls_timestamp_format(),
                                      session_info.get_local_nls_timestamp_tz_format(),
                                      session_info.get_tz_info_wrap(),
                                      arg.tablet_ids_,
                                      need_reorder_column_id))) {
      LOG_WARN("fail to init create hidden table arg", KR(ret));
    } else if (OB_FAIL(ObDDLServerClient::create_hidden_table(create_table_arg, create_table_res,
        res.snapshot_version_, res.data_format_version_, session_info))) {
      LOG_WARN("failed to create hidden table", KR(ret), K(create_table_arg));
    } else if (OB_UNLIKELY(res.snapshot_version_ <= 0 || res.data_format_version_ <= 0)) {
      ret = OB_ERR_UNEXPECTED;
      LOG_WARN("invalid snapshot version", K(ret), K(res));
    } else {
      res.dest_table_id_ = create_table_res.dest_table_id_;
      res.task_id_ = create_table_res.task_id_;
      res.schema_version_ = create_table_res.schema_version_;
      res.is_no_logging_ = create_table_res.is_no_logging_;
      LOG_INFO("succeed to create hidden table", K(arg), K(res));
    }
    THIS_WORKER.set_timeout_ts(origin_timeout_ts);
  }
  return ret;
}

int ObTableLoadRedefTable::finish(const ObTableLoadRedefTableFinishArg &arg,
                                  ObSQLSessionInfo &session_info)
{
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!arg.is_valid())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid args", KR(ret), K(arg));
  } else if (session_info.get_ddl_info().is_mview_complete_refresh()) {
    //pass
  } else {
    int64_t foreign_key_checks = 0;
    session_info.get_foreign_key_checks(foreign_key_checks);
    const int64_t origin_timeout_ts = THIS_WORKER.get_timeout_ts();
    ObCopyTableDependentsArg copy_table_dependents_arg;
    copy_table_dependents_arg.task_id_ = arg.task_id_;
    copy_table_dependents_arg.tenant_id_ = arg.tenant_id_;
    copy_table_dependents_arg.copy_indexes_ = true;
    copy_table_dependents_arg.copy_constraints_ = true;
    copy_table_dependents_arg.copy_triggers_ = false;
    copy_table_dependents_arg.copy_foreign_keys_ = is_oracle_mode() || (is_mysql_mode() && foreign_key_checks);
    copy_table_dependents_arg.ignore_errors_ = false;
    if (OB_FAIL(ObDDLServerClient::copy_table_dependents(copy_table_dependents_arg, session_info))) {
      LOG_WARN("failed to copy table dependents", KR(ret), K(copy_table_dependents_arg));
    } else {
      LOG_INFO("succeed to copy table dependents", K(copy_table_dependents_arg));
      ObFinishRedefTableArg finish_redef_table_arg;
      finish_redef_table_arg.task_id_ = arg.task_id_;
      finish_redef_table_arg.tenant_id_ = arg.tenant_id_;

      ObAddr rs_addr;
      ObDDLBuildSingleReplicaResponseArg build_single_replica_response_arg;
      build_single_replica_response_arg.task_id_             = arg.task_id_;
      build_single_replica_response_arg.tenant_id_           = arg.tenant_id_;
      build_single_replica_response_arg.dest_tenant_id_      = arg.tenant_id_;
      build_single_replica_response_arg.source_table_id_     = arg.table_id_;
      build_single_replica_response_arg.dest_schema_id_      = arg.dest_table_id_;
      build_single_replica_response_arg.schema_version_      = arg.schema_version_;
      build_single_replica_response_arg.dest_schema_version_ = arg.schema_version_;
      build_single_replica_response_arg.ls_id_               = share::ObLSID(1);
      build_single_replica_response_arg.dest_ls_id_          = share::ObLSID(1);
      build_single_replica_response_arg.tablet_id_           = ObTableID(-1);
      build_single_replica_response_arg.snapshot_version_    = 1;
      build_single_replica_response_arg.execution_id_        = 1;
      build_single_replica_response_arg.ret_code_            = ret;
      build_single_replica_response_arg.server_addr_ = GCTX.self_addr();
      if (OB_FAIL(ObDDLServerClient::finish_redef_table(
            finish_redef_table_arg, build_single_replica_response_arg, session_info))) {
        LOG_WARN("failed to finish redef table", KR(ret), K(finish_redef_table_arg));
        if (ret == OB_NOT_MASTER) { //sql cannot be retried here, so change errcode
          ret = OB_DIRECT_LOAD_COMMIT_ERROR;
        }
      } else {
        LOG_INFO("succeed to finish redef table", KR(ret), K(finish_redef_table_arg));
      }
    }
    THIS_WORKER.set_timeout_ts(origin_timeout_ts);
  }
  return ret;
}

int ObTableLoadRedefTable::abort(const ObTableLoadRedefTableAbortArg &arg,
                                 ObSQLSessionInfo &session_info)
{
  int ret = OB_SUCCESS;
  if (OB_UNLIKELY(!arg.is_valid())) {
    ret = OB_INVALID_ARGUMENT;
    LOG_WARN("invalid args", KR(ret), K(arg));
  } else if (session_info.get_ddl_info().is_mview_complete_refresh()) {
    //pass
  } else {
    const int64_t origin_timeout_ts = THIS_WORKER.get_timeout_ts();
    ObAbortRedefTableArg abort_redef_table_arg;
    abort_redef_table_arg.task_id_ = arg.task_id_;
    abort_redef_table_arg.tenant_id_ = arg.tenant_id_;
    if (OB_FAIL(ObDDLServerClient::abort_redef_table(abort_redef_table_arg, &session_info))) {
      LOG_WARN("failed to abort redef table", KR(ret), K(abort_redef_table_arg));
    } else {
      LOG_INFO("succeed to abort hidden table", K(arg));
    }
    THIS_WORKER.set_timeout_ts(origin_timeout_ts);
  }
  return ret;
}

}  // namespace observer
}  // namespace oceanbase
