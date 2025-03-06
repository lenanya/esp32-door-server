<?php
if (!isset($_GET["temp"])) {
    echo "get error";
    die(1);
}

$temp = $_GET["temp"];

fwrite(fopen("temp", "w"), $temp);

$data = json_decode(file_get_contents("temps.json"));

$data[] = ["temp" => $temp, "unix" => time()];

$hour = 3600;

$logamount = 48;

$logmaxtime = $hour * $logamount;

if (count($data) < $logmaxtime) {
    $last = $data;
} else {
    $last = array_slice($data, -$logmaxtime);
}
$file = fopen("temps.json", "w");
fwrite($file, json_encode($last));