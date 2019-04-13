function askRebootFirmware() {
    var ask = window.confirm("Are you sure you want to restart?");
    if (ask) {
        window.location.href = "/reboot";

    }
}
