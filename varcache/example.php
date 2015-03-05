<?php
//Besucherzähler mit Datum
require 'class.varcache.php';	//Klasse einbinden

$cache = new varCache('timestamp_next_scan');
$timestamp_from_file = 'next_run';
$intervall = 60; //in sekunden

$cache->lock();
if($cache->exists($timestamp_from_file)) {
    $timestamp = $cache->get($timestamp_from_file);
	if($timestamp+$intervall < time())
	{
		echo 'Timestamp veraltet muss neu ausgeführt werden<br>';
		$timestamp = time();
		echo 'Timestamp neu: ' . $timestamp;
	}
	else
	{
		$restzeit =  $intervall-(time()-($timestamp));
		echo 'Noch ' . $restzeit . ' Sekunden Zeit';
	}
}
else { 
	$timestamp = time();
	echo 'Neuer Timestamp angelegt' . $timestamp;
}

echo '<br><br>DEBUG <br>';
echo '$timestamp: ' . $timestamp. '<br>';
echo 'time(): ' . time(). '<br>';


$cache->set($timestamp_from_file, $timestamp);
$cache->unlock();
?>