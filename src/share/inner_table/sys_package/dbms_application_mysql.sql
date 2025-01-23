#package_name: dbms_application_info
#author: xiaoyi.xy
CREATE OR REPLACE PACKAGE DBMS_APPLICATION_INFO
  PROCEDURE READ_CLIENT_INFO(OUT CLIENT_INFO VARCHAR(65535));

  PROCEDURE READ_MODULE(OUT MODULE_NAME VARCHAR(65535), OUT ACTION_NAME VARCHAR(65535));

  PROCEDURE SET_ACTION(IN ACTION_NAME VARCHAR(65535));

  PROCEDURE SET_CLIENT_INFO(IN CLIENT_INFO VARCHAR(65535));

  PROCEDURE SET_MODULE(IN MODULE_NAME VARCHAR(65535), IN ACTION_NAME VARCHAR(65535));
END;

//
