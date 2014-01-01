<?php
include "common.php";
?>
<html>
<link href="main.css" type="text/css" rel="stylesheet">
<body>
<form name="input" action="tree.php" method="GET">
<?php
foreach ($gCommands as $cmd => $ignore) {
?>
<input type="submit" name="cmd" value="<?php echo $cmd;?>"><br/><br/>
<?php
}
?>
</form> 
</body></html>
