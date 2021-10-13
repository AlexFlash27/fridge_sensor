<?php

//%progdir%\modules\wget\bin\wget.exe -q --no-cache http://fridge/index.php // планировщик заданий

require 'vendor/autoload.php';

use PhpOffice\PhpSpreadsheet\Spreadsheet;
use PhpOffice\PhpSpreadsheet\Writer\Xlsx;
use PhpOffice\PhpSpreadsheet\Style\NumberFormat;
date_default_timezone_set('Etc/GMT-4');

    $host = '192.168.7.1';
    $username = 'root';
    $password = 'root';
    $dbname = 'yii2test';

    $conn = new mysqli($host, $username, $password, $dbname); // ПОДКЛЮЧЕНИЕ К БД

    if ($conn->connect_error) {
        die("Connection fail:" . $conn->connect_error);
    }

    $spreadsheet = new Spreadsheet();
    $writer = new Xlsx($spreadsheet);

    $spreadsheet->setActiveSheetIndex(0);
    $activeSheet = $spreadsheet->getActiveSheet();

    $activeSheet->getStyle('A1:E1')->getFont()->setBold(true);
    $activeSheet->setAutoFilter('A1:B11');

    $activeSheet->setCellValue('A1', '№ датчика');
    $activeSheet->setCellValue('B1', 'Произв. помещение-холод. оборудование');
    $activeSheet->setCellValue('C1', 'Дата и время');
    $activeSheet->setCellValue('D1', 'Температура °С');
    $activeSheet->setCellValue('E1', 'Влажность %');

    $query = $conn->query('SELECT * FROM 
		(SELECT * FROM 
		(SELECT * FROM arduino_test WHERE TIME(date) BETWEEN "20:00:00" AND "20:04:59"
		UNION
		SELECT * FROM arduino_test WHERE TIME(date) BETWEEN "05:30:00" AND "05:34:59") AS alias ORDER BY id DESC LIMIT 10) AS alias ORDER BY fridgeName ASC'); // ПОЛУЧАЕМ НУЖНЫЕ ЗНАЧЕНИЯ В СООТВЕТСТВИИ С ЗАДАННЫМ ВРЕМЕНЕМ 

    if ($query->num_rows > 0) {
        $i = 2;
        while ($row = $query->fetch_assoc()) {
            $activeSheet->setCellValue('A' . $i, $row['idSensor']);
            $activeSheet->setCellValue('B' . $i, $row['fridgeName']);
            $activeSheet->setCellValue('C' . $i, $row['date']);
            $activeSheet->setCellValue('D' . $i, $row['temperature']);
            $activeSheet->setCellValue('E' . $i, $row['humidity']);
            $i++;
        }
    }

    /*$activeSheet->getStyle('B') 
    ->getNumberFormat()
    ->setFormatCode(NumberFormat::FORMAT_DATE_DDMMYYYY);*/

    $activeSheet->getColumnDimension('A')->setWidth(15); // ЗАДАЕМ ШИРИНУ КОЛОНОК
    $activeSheet->getColumnDimension('B')->setWidth(41);
    $activeSheet->getColumnDimension('C')->setWidth(18);
    $activeSheet->getColumnDimension('D')->setWidth(13);
    $activeSheet->getColumnDimension('E')->setWidth(11);

    $cellCLowest = $activeSheet->getCell('C2'); // ПОЛУЧАЕМ ПЕРВУЮ СТРОКУ С ДАННЫМИ
    $cellCHighest = $activeSheet->getHighestRow('C'); // ПОЛУЧАЕМ ПОСЛЕДНЮЮ СТРОКУ С ДАННЫМИ

    $cellCLowestValue = substr($cellCLowest->getValue(),0,10); // УБИРАЕМ ВРЕМЯ ИЗ ДАТЫ
    $cellCHighestValue = substr($activeSheet->getCell('C' . $cellCHighest)->getValue(),0,10); // ПОЛУЧАЕМ ЗНАЧЕНИЕ ПОСЛЕДНЕЙ СТРОКИ С ДАННЫМИ И УБИРАЕМ ВРЕМЯ ИЗ ДАТЫ

    $filename = 'журнал за ' . $cellCLowestValue . ':' . $cellCHighestValue . '.xlsx'; //ЗАДАЕМ ИМЯ ФАЙЛА

    $writer = new Xlsx($spreadsheet);
    $writer->save('reports/' . $filename); // СОХРАНЯЕМ ФАЙЛ В ПАПКУ reports
    
    ////////////////ОТПРАВКА НА ПОЧТУ////////////////    
    $transport = (new Swift_SmtpTransport('smtp.gmail.com', 587, 'tls')) // ДАННЫЕ ДЛЯ ОТПРАВКИ ПИСЕМ
        ->setUsername('alexflash27')
        ->setPassword('ouqwpzhnvqzoobkw');
 
    $mailer = new Swift_Mailer($transport);
 
    $message = new Swift_Message(); // СОЗДАЕМ СООБЩЕНИЕ
 
    $filenameSubject = substr($filename,16,-5);
    $message->setSubject('Журнал температур холодильного оборудования за ' . $filenameSubject); // ТЕМА ПИСЬМА
 
    $message->setFrom(['alexflash27@gmail.com' => 'Admin']); // ОТ КОГО ОТПРАВЛЯЕМ ПИСЬМО
 
    $message->setTo(['alexflash27@gmail.com']); // КОМУ ОТПРАВЛЯЕМ ПИСЬМО
 
    $message->attach(Swift_Attachment::fromPath('reports/' . $filename)); // ПРИКРЕПЛЯЕМ ФАЙЛ
      
    $result = $mailer->send($message); // ОТПРАВЛЯЕМ ПИСЬМО
 