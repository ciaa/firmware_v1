<?php
/* get tasks */
$pdefs = $config->getList("/TEMPLATE","PDEF");
?>
/* there are <?=count($pdefs);?> pdefs */
<?php

$count = 1;
foreach ($pdefs as $pdef)
{
?>
   /* the pdef <?=$count;?> contains named <?=$pdef;?>: */
<?php
   print "        /* PARAM1: " . $config->getValue("/TEMPLATE/" . $pdef, "PARAM1") . " - PARAM2: " . $config->getValue("/TEMPLATE/" . $pdef, "PARAM2") . " */\n";
   $count++;
}

?>
