<?php
include "php_serial.class.php";
include "common.php";

$command = "";

$cmd = $_GET["cmd"];
if ($cmd != "" && array_key_exists($cmd, $gCommands)) {
  $command = $gCommands[$cmd];
}
else {
  echo "error";
  return;
}

$serial = new phpSerial;

$serial->deviceSet("/dev/ttyACM0");
$serial->confBaudRate(115200);
$serial->confParity("none");
$serial->confCharacterLength(8);
$serial->confStopBits(1);
$serial->confFlowControl("none");

$serial->deviceOpen();
sleep(1);
for ($i = 0 ; $i < strlen($command) ; $i++) {
	$serial->sendMessage($command[$i]);
}
$serial->deviceClose();

header('Refresh: 1; URL=/');
echo "Done ".$command;
?>
