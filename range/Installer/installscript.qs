function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();

    if (installer.value("os") === "win") {
        var userProfile = installer.environmentVariable("USERPROFILE");
        installer.setValue("UserProfile", userProfile);
        component.addOperation("CreateShortcut",
                               "@TargetDir@/bin/Range.exe",
                               "@StartMenuDir@/Range.lnk",
                               "workingDirectory=@TargetDir@",
                               "description=Range");
        component.addOperation("CreateShortcut",
                               "@TargetDir@/bin/Range.exe",
                               "@UserProfile@/Desktop/Range.lnk",
                               "workingDirectory=@TargetDir@",
                               "description=Range");
        component.addOperation("CreateShortcut",
                               "@TargetDir@/maintenancetool.exe",
                               "@StartMenuDir@/Range Maintenance tool.lnk",
                               "workingDirectory=@TargetDir@",
                               "description=Range Maintenance tool");
    }

    if (installer.value("os") === "x11") {
        component.addOperation("CreateDesktopEntry",
                               "Range.desktop",
                               "Type=Application\nName=Range\nExec=@TargetDir@/bin/Range\nPath=@TargetDir@/bin\nTerminal=false\nIcon=range");
        component.addOperation("InstallIcons",
                               "@TargetDir@/icons");
    }
}
