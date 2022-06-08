VERSION=3.2.7
YEAR=2022
RELEASEDATE=2022-03-07
NAME='Range Software'
SHORTNAME='Range'
TITLE='Finite Element Analysis'
DESCRIPTION='Engineering simulation software'
AUTHOR='Tomas Soltys'
AUTHORADDRESS='Ke Zlatkovu 597, 25070 Odolena Voda, Czech Republic'
EMAIL='tomas.soltys@range-software.com'
WWW='https://www.range-software.com'

DEFINES += VERSION=\\\"$$VERSION\\\" \
           YEAR=$$YEAR \
           NAME=\"\\\"$$NAME\\\"\" \
           SHORTNAME=\"\\\"$$SHORTNAME\\\"\" \
           TITLE=\"\\\"$$TITLE\\\"\" \
           DESCRIPTION=\"\\\"$$DESCRIPTION\\\"\" \
           AUTHOR=\"\\\"$$AUTHOR\\\"\" \
           EMAIL=\"\\\"$$EMAIL\\\"\" \
           WWW=\"\\\"$$WWW\\\"\" \

DEBUG_EXT = ""
INSTALLER_PATH = "$${OUT_PWD}/../installer_$${VERSION}"
INSTALLER_CONF_DIR_PATH = $${INSTALLER_PATH}/conf
INSTALLER_META_DIR_PATH = $${INSTALLER_PATH}/packages/$${SHORTNAME}/meta
INSTALLER_DATA_DIR_PATH = $${INSTALLER_PATH}/packages/$${SHORTNAME}/data

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
    DEBUG_EXT = "_debug"
}
