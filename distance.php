<?php
if (!isset($_GET["distance"])) {
    echo "get error";
    die(1);
}

function stohms($s): string {
    $hours = floor($s / 3600);
    $minutes = floor(($s % 3600) / 60);
    $seconds = $s - $hours * 3600 - $minutes * 60;
    if ($s >= 3600) {
        return $hours."h ".$minutes."m ".$seconds."s";
    } else if ($s >= 60) {
        return $minutes."m ".$seconds."s";
    } else {
        return $seconds."s";
    }
}

$distance = $_GET["distance"];

date_default_timezone_set("Europe/Berlin");

$data = json_decode(file_get_contents("lastopen.json"));
$time = time() - end($data)->unix;
fwrite(fopen("time", "w"), $time);

if (!$distance) {
    echo "AUGH";
    die(1);
}

$hour = 3600;

$logamount = 24;

$logmaxtime = $hour * $logamount;

if ($distance > 70) {
    $open = file_get_contents("open");
    if ($open == "closed" && $time > 10) {
        $data[] = ["distance" => intval($distance), 
            "time" => "".date("d-m-Y - H:i:s"), 
            "difference" => stohms($time), 
            "difference_seconds" => $time, "unix" => time()
        ];
        if (count($data) < $logmaxtime) {
            $last = $data;
        } else {
            $last = array_slice($data, -$logmaxtime);
        }
        $file = fopen("lastopen.json", "w");
        fwrite($file, json_encode($last));
    }

    fwrite(fopen("open", "w"), "open");
    
} else {
    fwrite(fopen("open", "w"), "closed");
}

$data = json_decode(file_get_contents("distances.json"));
$data[] = ["distance" => intval($distance), "time" => "".date("d.m - H:i:s")];

$file = fopen("distances.json", "w");

$last = [];

$last = (count($data) < $logmaxtime) ? array_slice($data, -$logmaxtime) :  $data;

$json = json_encode($last);

fwrite($file, $json);