<?php
require __DIR__ . '/../vendor/autoload.php';
require __DIR__ . '/../checkToken.php';
$parts = parse_url( $_SERVER['REQUEST_URI']);
parse_str($parts['query'], $query);
if(!$_GET['token']){
    header('X-PHP-Response-Code: 400', true, 400);
    header('Content-Type: application/json; charset=utf-8');
    $data = [
        "error" => "not_token",
        "message" => "Not Token"
    ];
    echo json_encode($data);
    return;
}
$result = checkToken($query['token'],$_SERVER['PHP_SELF']);
if (!$result[0]) {
    header('X-PHP-Response-Code: 403', true, 403);
    header('Content-Type: application/json; charset=utf-8');
    echo json_encode($result[1]);
    return;
}
$result=[];
//Сюда вставиться массив каналов, именно сюда в 24 строчку


foreach ($result as $key => $val) {
	echo $val."&user_token=".$query['token'];
}
 header('Content-Type: text/plain; charset=windows-1251');
?>
