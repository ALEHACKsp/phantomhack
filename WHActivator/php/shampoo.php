<?php
include ('config.php');

$dbc = mysqli_connect($db_host, $db_user, $db_pass, $db_name);

if (!$dbc) 
{
    echo 5;
    exit;
}


function RemoveMultipleSpaces($str)
{
    $output = preg_replace('!\s+!', ' ', $str);
    return $output;
}

function SQLPrepareString($str)
{
	global $dbc;
    $output = RemoveMultipleSpaces($str);
    $output = addslashes($output);
    $output = htmlspecialchars($output);
    $output = stripslashes($output);
    $output = mysqli_real_escape_string($dbc, $output);
    return $output;
}

function CheckUserExist($uid)
{
	global $dbc;
	if($result = $dbc->query("SELECT * FROM users WHERE uid='" . $uid . "'")) 
	{
		if($result->num_rows > 0)
			$r = 1;
		else
			$r = 0;
		
	}
	else
		$r = 2;
	
    $result->close();
	return $r;
}

function CheckUsernameExist($un)
{
	global $dbc;
	if($result = $dbc->query("SELECT * FROM users WHERE username='" . $un . "'")) 
	{
		if($result->num_rows > 0)
			$r = 1;
		else
			$r = 0;
		
	}
	else
		$r = 2;
	
    $result->close();
	return $r;
}

function CheckEmailExist($email)
{
	global $dbc;
	if($result = $dbc->query("SELECT * FROM users WHERE email='" . $email . "'")) 
	{
		if($result->num_rows > 0)
			$r = 1;
		else
			$r = 0;
		
	}
	else
		$r = 2;
	
    $result->close();
	return $r;
}


function CheckUserActivation($uid)
{
	global $dbc;
	
	if($result = $dbc->query("SELECT * FROM users WHERE uid='" . $uid . "'")) 
	{
		$row = $result->fetch_assoc();
		$rdel = $row['rdel'];
	}
	else
	{
		$r = 2;
		$result->close();
		return $r;
	}
	
	if($rdel == 1)
	{
		$r = 3;
		$result->close();
		return $r;
	}
	
	$result->close();
	if($result = $dbc->query("SELECT * FROM users WHERE uid='" . $uid . "'")) 
	{
		$finfo = $result->fetch_assoc();
		$activated = $row['activated'];
		if($activated == 1)
			$r = 1;
		else 
			$r = 0;
	}
	else
		$r = 2;
	
    $result->close();
	return $r;
}

function GetUserName($uid)
{
	global $dbc;
	
	if($result = $dbc->query("SELECT * FROM users WHERE uid='" . $uid . "'")) 
	{
		$row = $result->fetch_assoc();
		return $row['username'];
	}
	else
		return "";
}

function CheckVersion($version)
{
	global $dbc;
	
	if($result = $dbc->query("SELECT * FROM whdata WHERE field='LastVersion'")) 
	{
		$row = $result->fetch_assoc();
		$lv = $row['fieldval'];
	}
	else
		return "";
	
		return $lv;
}

function GetField($fieldname)
{
	global $dbc;
	
	if($result = $dbc->query("SELECT * FROM whdata WHERE field='" . $fieldname . "'")) 
	{
		$row = $result->fetch_assoc();
		$val = $row['fieldval'];
	}
	else
		return 0;
	
		return $val;
}


function GetActivationKey($uid)
{
	$part1 = ""; $part2 = "";
	$mix = "";
	$str = "";

	for ($i = 0; $i < 16; $i++)
		$part1[$i] = $uid[$i];
	$part1 = implode("", $part1);

	for ($i = 16; $i < 32; $i++)
		$part2[$i-16] = $uid[$i];
	$part2 = implode("", $part2);

	$j = 0; $k = 0;
	for ($i = 0; $i < 32; $i++)
	{
		if ($i % 2 == 0)
		{
			$mix[$i] = $part2[$j];
			$j++;
		}
		else
		{
			$mix[$i] = $part1[$k];
			$k++;
		}
	}
	$mix = implode("", $mix);

	$str  = md5(md5(md5($part1))) . md5($mix) . md5(md5($part2)); 
	
	return $str;
}

function CreateNewUser($uid, $username, $pass, $email, $ip)
{
	global $dbc;
	
	$req = "INSERT INTO users (uid, ip, username, password, email) VALUES ('" . $uid . "', '" . $ip . "', '" . $username . "', '" . $pass . "', '" . $email . "')";
	if($result = $dbc->query($req)) 
		$r = 1;
	else
		$r = 2;
	
	return $r;
}

function GetIP()
{
	$user_ip = $_SERVER["REMOTE_ADDR"];
	if (isset($_SERVER["HTTP_X_FORWARDED_FOR"])) 
    {
		$user_ip = $_SERVER["HTTP_X_FORWARDED_FOR"];
	} else if (isset($_SERVER["HTTP_CLIENT_IP"])) 
    {
		$user_ip = $_SERVER["HTTP_CLIENT_IP"];
	} else 
    {
		return $user_ip;
	}
	$ips = preg_split('/[, ]/', $user_ip);
	foreach ($ips as $ip) 
    {
		if (preg_match('/^(\d{1,3}\.){3}\d{1,3}$/s', $ip)
			&& !isPrivateIP($ip) ) 
        {
			$user_ip = $ip;
		}
	}
	return $user_ip;
}

function GetRegistrationKey($ip, $uid)
{
	$str = $ip . $uid;
	$str = md5($str) . md5(md5($uid));
	$str = md5($str);
	return $str;
}



if(isset($_POST[md5('ucheck')]))
{
	$ver = SQLPrepareString($_POST[md5('ucheck')]);
	$lv = CheckVersion($ver);
	if($lv == "")
		echo 0;
	else
		echo $lv;
	die();
}

if(isset($_POST[md5('getsize')]))
{
	$param = $_POST[md5('getsize')];
	if($param == md5("activator"))
	{
		echo filesize($a_path);
	}
	else
		echo 0;
	die();
}


if(isset($_POST[md5('getip')]))
{
	if($_POST[md5('getip')] == 1)	
		echo GetIP();
	else
		echo 0;
	
	die();
}

if(isset($_POST[md5('getusr')]))
{
	$uid = SQLPrepareString($_POST[md5('getusr')]);
	if(CheckUserExist($uid) == 1)
	{
		$usr = GetUserName($uid);
		if($usr == "")
		{
			echo "UNKNOWN";
			die();
		}
		
		echo $usr;
		die();
	}
	else
	{
		echo "UNKNOWN";
		die();
	}
	
}

if(isset($_GET[md5('getupd')]))
{
	$uid = SQLPrepareString($_GET[md5('getupd')]);
	
	$ue = CheckUserExist($uid);
	if($ue == 0 || $ue == 2)
	{
		echo 0;
		die();
	}
	
	$activation = CheckUserActivation($uid);
	if($activation == 0 || $activation == 2)
	{
		echo 0;
		die();
	}
	
	header('Content-Disposition: attachment; charset=windows-1252; filename=' . basename($a_path));
	readfile($a_path);
	
	die();
}

if(isset($_POST[md5('reg')]))
{
	$reg = $_POST[md5('reg')];
	$uid = SQLPrepareString($_POST[md5('uid')]);
	$username = SQLPrepareString($_POST[md5('username')]);
	$pass = SQLPrepareString($_POST[md5('pass')]);
	$email = SQLPrepareString($_POST[md5('email')]);
	$ip = GetIP();
	
	if($reg != GetRegistrationKey($ip, $uid))
	{
		echo 5;
		die();
	}
	
	$usr_exists = CheckUsernameExist($username);
	$uid_exists = CheckUserExist($uid);
	$email_exists = CheckEmailExist($email);
	if($usr_exists == 1)
	{
		echo 2;
		die();
	}
	
	if($uid_exists == 1)
	{
		echo 3;
		die();
	}
	
	if($email_exists == 1)
	{
		echo 4;
		die();
	}
	
	if($usr_exists == 2 || $uid_exists == 2 || $email_exists == 2)
	{
		echo 0;
		die();
	}
	
	$rs = CreateNewUser($uid, $username, $pass, $email, $ip);
	
	if($rs == 2)
	{
		echo 0;
		die();
	}
	
	echo 1;
	die();
}


if(isset($_POST[md5('uid')]))
{
	$uid = SQLPrepareString($_POST[md5('uid')]);
	$ue = CheckUserExist($uid);
	
	if($ue == 0)
	{
		echo 1;
		die();
	}
	if($ue == 2)
	{
		echo 0;
		die();
	}
	
	$activation = CheckUserActivation($uid);
	
	if($activation == 0)
	{
		echo 2;
		die();
	}
	if($activation == 2)
	{	
		echo 0;
		die();
	}
	if($activation == 3)
	{
		echo 3;
		die();
	}
	
	echo GetActivationKey($uid);
    die();
}


echo 0;

mysqli_close($dbc);
?>