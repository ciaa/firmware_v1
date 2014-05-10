<?php

require_once ("config.php");

$config->parseOilFile("example.oil");

#$config->dump();

print "Tasks: " . $config->getCount("/OSEK","TASK") . "\n";
print "TasksK List:\n";

$tasks = $config->getList("/OSEK","TASK");

$task_count=0;

foreach ($tasks as $task)
{
	print "Task " . $task_count++ . " Location " . $task . "\n";
	$attributes = $config->getAttributes($task,"*");
	foreach ($attributes as $attr)
	{
		print $attr . ": " . $config->getValue($task, $attr) . "\n";
	}
}

?>
