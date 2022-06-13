include(../range.pri)

CONFIG += file_copies

TEMPLATE = aux

config.path = $${INSTALLER_CONF_DIR_PATH}
config.files = config.xml

meta.path = $${INSTALLER_META_DIR_PATH}
meta.files = package.xml installscript.qs LICENSE.txt

COPIES += config meta

BIN_DIR_PATH = $${INSTALLER_DATA_DIR_PATH}/bin

APP_NAME = Range
TOOL_NAME = RangeSolver

CONFIG(debug, debug|release) {
    APP_NAME = $$join(APP_NAME,,,$${DEBUG_EXT})
    TOOL_NAME = $$join(TOOL_NAME,,,$${DEBUG_EXT})
}

INSTALLER_EXTENSION = 
win* {
    INSTALLER_EXTENSION = ".exe"
}
else {
    macx {
        INSTALLER_EXTENSION = ".app"
    }
    else {
        INSTALLER_EXTENSION = ".run"
    }
}
INSTALLER_BASE = $${APP_NAME}-$${VERSION}-installer
INSTALLER_NAME = $${INSTALLER_BASE}$${INSTALLER_EXTENSION}

INSTALLER_COMMAND = "binarycreator --offline-only -c $${INSTALLER_CONF_DIR_PATH}/config.xml -p $${INSTALLER_PATH}/packages $${INSTALLER_NAME}"
macx {
    DMG_RM_COMMAND = "rm -f $${INSTALLER_BASE}.dmg"
    INSTALLER_COMMAND = $$join(INSTALLER_COMMAND,,," && ")
    INSTALLER_COMMAND = $$join(INSTALLER_COMMAND,,,$${DMG_RM_COMMAND})
    DMG_COMMAND = "create-dmg --volname '$${APP_NAME}' $${INSTALLER_BASE}.dmg '$${INSTALLER_NAME}'"
    INSTALLER_COMMAND = $$join(INSTALLER_COMMAND,,," && ")
    INSTALLER_COMMAND = $$join(INSTALLER_COMMAND,,,$${DMG_COMMAND})
}

installer.target = $${INSTALLER_NAME}
installer.path = $${INSTALLER_PATH}
installer.commands = $${INSTALLER_COMMAND}
installer.depends = install_build install_deploy

DEPLOY_COMMAND =
win* {
    APP_NAME = $$join(APP_NAME,,,".exe")
    TOOL_NAME = $$join(TOOL_NAME,,,".exe")
    DEPLOY_COMMAND = $$[QT_INSTALL_PREFIX]/bin/windeployqt $${BIN_DIR_PATH}/$${APP_NAME}
}
else {
    macx {
        APP_NAME = $$join(APP_NAME,,,".app")
        DEPLOY_COMMAND = $$[QT_INSTALL_PREFIX]/bin/macdeployqt $${BIN_DIR_PATH}/$${APP_NAME} -executable=$${BIN_DIR_PATH}/$${TOOL_NAME} -verbose=2
    }
    else {
        DEPLOY_COMMAND = $${_PRO_FILE_PWD_}/../../scripts/linux_deploy_qt.sh --executable=$${BIN_DIR_PATH}/$${APP_NAME} --install-to=$${INSTALLER_DATA_DIR_PATH} --only-qt --qt-path=$$[QT_INSTALL_PREFIX]
    }
}

deploy.path = $${INSTALLER_DATA_DIR_PATH}
deploy.extra = $${DEPLOY_COMMAND}

STREAM_EDITOR_COMMAND = $$QMAKE_STREAM_EDITOR \
    ' -e "s/[{]VERSION[}]/$${VERSION}/g"' \
    ' -e "s/[{]YEAR[}]/$${YEAR}/g"' \
    ' -e "s/[{]RELEASEDATE[}]/$${RELEASEDATE}/g"' \
    ' -e "s/[{]NAME[}]/$${NAME}/g"' \
    ' -e "s/[{]SHORTNAME[}]/$${SHORTNAME}/g"' \
    ' -e "s/[{]TITLE[}]/$${TITLE}/g"' \
    ' -e "s/[{]DESCRIPTION[}]/$${DESCRIPTION}/g"' \
    ' -e "s/[{]AUTHOR[}]/$${AUTHOR}/g"' \
    ' -e "s/[{]AUTHORADDRESS[}]/$$replace(AUTHORADDRESS,'/','\/')/g"' \
    ' -e "s/[{]EMAIL[}]/$${EMAIL}/g"' \
    ' -e "s/[{]WWW[}]/$$replace(WWW,'/','\/')/g"'

!win* {
    STREAM_EDITOR_COMMAND = $$join(STREAM_EDITOR_COMMAND,,," -i")
    macx {
        STREAM_EDITOR_COMMAND = $$join(STREAM_EDITOR_COMMAND,,," --")
    }
}

BUILD_FILES =
for (CONFIG_FILE,config.files) {
    BUILD_FILES += $$shell_path($$config.path/$${CONFIG_FILE})
}
for (META_FILE,meta.files) {
    BUILD_FILES += $$shell_path($$meta.path/$${META_FILE})
}

BUILD_COMMAND =
for (BUILD_FILE, BUILD_FILES) {
    win* {
        TMP_BUILD_FILE = $$join(BUILD_FILE,,,"_tmp")
        BUILD_COMMAND += $$STREAM_EDITOR_COMMAND $${BUILD_FILE} > $${TMP_BUILD_FILE} $$escape_expand(\n\t)
        BUILD_COMMAND += $$QMAKE_MOVE $${TMP_BUILD_FILE} $${BUILD_FILE} $$escape_expand(\n\t)
    }
    else {
        BUILD_COMMAND += $$STREAM_EDITOR_COMMAND $${BUILD_FILE} $$escape_expand(\n\t)
    }
}

build.path = $${INSTALLER_CONF_DIR_PATH}
build.extra = $${BUILD_COMMAND}

INSTALLS += build deploy installer

DISTFILES += \
    installscript.qs
