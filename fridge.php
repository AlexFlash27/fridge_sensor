<?php
//header("Refresh: 0"); // Refresh the webpage every 10 seconds

//%progdir%\modules\wget\bin\wget.exe -q --no-cache http://fridge/index.php //планировщик заданий

require 'vendor/autoload.php';

use PhpOffice\PhpSpreadsheet\Spreadsheet;
use PhpOffice\PhpSpreadsheet\Writer\Xlsx;
use PhpOffice\PhpSpreadsheet\Style\NumberFormat;
date_default_timezone_set('Etc/GMT-4');

//if(date('l') == 'Thursday' && date("H:i:s") == '11:11:00') {
    $host = '192.168.7.1';
    $username = 'root';
    $password = 'root';
    $dbname = 'yii2test';

    $conn = new mysqli($host, $username, $password, $dbname);

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
    $activeSheet->setCellValue('D1', 'Температура');
    $activeSheet->setCellValue('E1', 'Влажность');

    $query = $conn->query('SELECT * FROM 
		(SELECT * FROM 
		(SELECT * FROM arduino_test WHERE TIME(date) BETWEEN "20:00:00" AND "20:00:59" 
		UNION
		SELECT * FROM arduino_test WHERE TIME(date) BETWEEN "05:30:00" AND "05:30:59") AS alias ORDER BY id DESC LIMIT 10) AS alias ORDER BY id ASC');

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

    $activeSheet->getColumnDimension('A')->setWidth(15);
    $activeSheet->getColumnDimension('B')->setWidth(41);
    $activeSheet->getColumnDimension('C')->setWidth(18);
    $activeSheet->getColumnDimension('D')->setWidth(13);
    $activeSheet->getColumnDimension('E')->setWidth(11);

    $cellC2 = $activeSheet->getCell('C2');
    $cellC11 = $activeSheet->getCell('C11');

    $cellC2Value = substr($cellC2->getValue(),0,10);
    $cellC11Value = substr($cellC11->getValue(),0,10);

    //$filename = 'отчет ' . date("d.m.Y") . '.xlsx';
    $filename = 'отчет ' . $cellC2Value . ':' . $cellC11Value . '.xlsx';

    $writer = new Xlsx($spreadsheet);
    $writer->save('reports/' . $filename);
    //$writer->save('C:/Users/user/Desktop/backup/' . $filename);

    ////////////////ОТПРАВКА НА ПОЧТУ////////////////    
    // Create the SMTP Transport
    $transport = (new Swift_SmtpTransport('smtp.gmail.com', 587, 'tls'))
        ->setUsername('alexflash27')
        ->setPassword('ouqwpzhnvqzoobkw');
 
    // Create the Mailer using your created Transport
    $mailer = new Swift_Mailer($transport);
 
    // Create a message
    $message = new Swift_Message();
 
    // Set a "subject"
    /*$filenameSubject = substr($filename,0,-5);*/
    $message->setSubject('отчет');
 
    // Set the "From address"
    $message->setFrom(['alexflash27@gmail.com' => 'Admin']);
 
    // Set the "To address" [Use setTo method for multiple recipients, argument should be array]
    $message->addTo('alexflash27@yandex.ru');
 
    // Add an "Attachment" (Also, the dynamic data can be attached)
    $message->attach(Swift_Attachment::fromPath('reports/' . $filename));
      
    // Send the message
    $result = $mailer->send($message);
//}