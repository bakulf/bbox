<?php
// Copyright (c) Day Barr 2007.  All Rights Reserved
//
// Version:        1.0
// License:        Creative Commons Attribution-ShareAlike 2.0 License
//                 http://creativecommons.org/licenses/by-sa/2.0/
// Note:           If you change or improve on this script, please let me know by 
//                 contacting me: http://daybarr.com/contact
// 
// LIMITATIONS:
// 1) Doesn't make use of svn log's --limit switch, as this did not exist in the 
// version of svn originally used.  Instead, a date is used to limit output to log
// messages since that date.
// 2) Doesn't work with svn repositories that require a username/password
// 3) Does no caching at all! Each hit on the feed URL results in a run of svn log! 
// So do not deploy this on a public server!
//
// ADVANTAGES:
// Simple install - assuming you have the right setup, just put this file in your 
// public_html directory, point your browser at
// http://server/~username/svnfeed.php and it should work

// CHANGE THIS TO POINT TO THE SVN BINARY ON YOUR SERVER - NEEDS FULL PATH
$SVN_PATH = '/usr/bin/svn';

// The default URL given in the "build a feed" form.
$DEFAULT_URL = 'http://storage.nois3lab.it/bbox';

// Set true to run in demo mode - no svn calls are done and an example feed is output instead.
// Set false to run normally - requested svn log is actually retrieved and parsed
$DEMO_MODE = false; // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< turn off

error_reporting(E_ALL);

# Undo the evil done by "magic quotes" if it's turned on
if (get_magic_quotes_gpc() == 1)
{
    function stripslashes_deep($value)
    {
        $value = is_array($value) ?
                 array_map('stripslashes_deep', $value) :
                 stripslashes($value);
        return $value;
    }

    $_REQUEST = array_map('stripslashes_deep', $_REQUEST);
    $_POST = array_map('stripslashes_deep', $_POST);
    $_GET = array_map('stripslashes_deep', $_GET);
    $_COOKIE = array_map('stripslashes_deep', $_COOKIE);
    $_SERVER = array_map('stripslashes_deep', $_SERVER);
}

parse_str(@$_SERVER['QUERY_STRING'], $qs);
$url = @$qs['url'];

if (!isset($url))
{
    print '<?xml version="1.0" encoding="utf-8"?>'."\r\n";
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US" lang="en-US">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="robots" content="noindex,nofollow">
<title>Subversion log => feed</title>
<style type="text/css">
body {
 font: 11pt "Lucida Grande", "Trebuchet MS", Arial, Helvetica, sans-serif;
 color: black;
 background: white;
}
dt {
 width: 20em;
 float: left;
 clear: left;
}
dd {
 padding: 5px;
 margin: 0;
}
input {margin: 0;clear:left;}
input[type='text']:focus {background: #ffc;}
select:focus {background: #ffc;}
label {
 text-align: right;
 width: 250px;
 padding-right: 20px;
}
</style>
</head>
<body>
<h1>Subversion Log Feed Generator</h1>
<p>Create an <a href="http://atomenabled.org/" title="The Atom Enabled Alliance">Atom</a> feed from a <a href="http://subversion.tigris.org/" title="Subversion Project">Subversion</a> log.</p>
<?php
    if ($DEMO_MODE)
    {
?>
<p><strong>Running in Demo Mode</strong></p>
<?php
    }
?>
<form method="get" action="<?php print $_SERVER['PHP_SELF']; ?>">
<dl>
<dt>
<label for="url" accesskey="U">Subversion URL to show log for: </label>
</dt>
<dd>
<input type="text" name="url" id="url" size="50" value="<?php print $DEFAULT_URL; ?>" />
</dd>
<dt>
<label for="title" accesskey="T">Title to give to the resulting feed: </label>
</dt>
<dd>
<input type="text" name="title" id="title" size="50" value="Svn: Commits for example" />
</dd>
<dt>
<label for="number" accesskey="N">Show log for the past: </label>
</dt>
<dd>
<input type="text" name="number" id="number" size="3" value="7" />
<select name="unit">
<option value="days" selected="selected">days</option>
<option value="weeks">weeks</option>
<option value="months">months</option>
<option value="years">years</option>
</select>
</dd>
<dt>
<label for="verbose" accesskey="V">Include a list of modified files for each entry: </label>
</dt>
<dd>
<input type="checkbox" name="verbose" id="verbose" />
</dd>
<dt></dt>
<dd>
<input type="submit" value="Make feed" />
</dd>
</dl>
</form>
</body>
</html>
<?php
    exit();
}

$cmd = "$SVN_PATH log --xml --stop-on-copy ".escapeshellcmd($url);

if (array_key_exists('number', $qs) && array_key_exists('unit', $qs))
{
    $qs['rev'] = '{'.date('Y-m-d', strtotime($qs['number'].' '.$qs['unit'].' ago')).'}:HEAD';
}
if (strlen(@$qs['rev']))
{
    $cmd .= ' -r '.escapeshellcmd($qs['rev']);
}
if (array_key_exists('verbose', $qs))
{
    $cmd .= ' -v';
}

$logRetriever = null;

if ($DEMO_MODE)
{
    class DemoLogRetriever
    {
        var $_done;
        var $_cmd;

        // @ctor
        function DemoLogRetriever($cmd)
        {
            $this->_done = false;
            $this->_cmd = $cmd;
        }

        function Data()
        {
            $data = $this->_done ? null :
               '<?xml version="1.0"?>
                <log>
                <logentry revision="2">
                <author>Day Barr</author>
                <date>2007-06-30T00:00:00.000000Z</date>
                <msg>This is only a demo.  If this was not running in demo mode, you would be seeing a feed containing an item for every entry in the log produced by the following svn command:
                
                '.htmlspecialchars($this->_cmd).'</msg>
                </logentry>
                <logentry revision="1">
                <author>Day Barr</author>
                <date>2007-06-29T00:00:00.000000Z</date>
                <paths>
                <path action="A">/trunk/newfile.php</path>
                <path action="D">/trunk/removedfile.txt</path>
                <path action="M">/trunk/changedfile.png</path>
                </paths>
                <msg>When the Verbose flag is used, you will also get information on the files changed along with each commit message</msg>
                </logentry>
                </log>';
            $this->_done = true;
            return $data;
        }

        function EndOfData()
        {
            return $this->_done;
        }

        function Errors()
        {
            return null;
        }

        function Close()
        {
            return false;
        }
    }
    $logRetriever = new DemoLogRetriever($cmd);
}
else
{
    class SvnLogRetriever
    {
        var $_pipes;
        var $_process;

        // @ctor
        function SvnLogRetriever($cmd)
        {
            $descriptorspec = array
            (
               0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
               1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
               2 => array("pipe", "w")   // stderr is a pipe that the child will write to
            );
            
            $this->_process = proc_open($cmd, $descriptorspec, $this->_pipes);

            if (!is_resource($this->_process))
            {
                die('Failed to launch process');
            }
            
            // Write to the child process (svn) and send it a t<CR><LF> to 
            // (t)emporarily accept any certificates it may require - 
            // otherwise it will block us indefinitely
            fwrite($this->_pipes[0], "t\r\n"); 
        }

        function Data()
        {
            return fread($this->_pipes[1], 4096);
        }

        function EndOfData()
        {
            return feof($this->_pipes[1]);
        }

        function Errors()
        {
            return fread($this->_pipes[2], 4096);
        }

        function Close()
        {
            fclose($this->_pipes[0]);
            fclose($this->_pipes[1]);
            fclose($this->_pipes[2]);
            return proc_close($this->_process);
        }
    }
    $logRetriever = new SvnLogRetriever($cmd);
}

$logEntry = array();
$node = '';
$paths = array();

Header('Content-Type: text/xml');

$thisurl = $_SERVER['PHP_SELF']; // 'http://'.htmlspecialchars($_SERVER['SERVER_NAME'].$_SERVER['REQUEST_URI']);

$xml_parser = xml_parser_create();
// use case-folding so we are sure to find the tag in $map_array
xml_parser_set_option($xml_parser, XML_OPTION_CASE_FOLDING, false);
xml_set_element_handler($xml_parser, "startElement", "endElement");
xml_set_character_data_handler($xml_parser, "characterData");

while ($data = $logRetriever->Data())
{
    if (!xml_parse($xml_parser, $data, $logRetriever->EndOfData())) 
    {
        die(sprintf("XML error: %s at line %d",
                    xml_error_string(xml_get_error_code($xml_parser)),
                    xml_get_current_line_number($xml_parser)));
    }
}
xml_parser_free($xml_parser);

$errorOutput = '';
while ($data = $logRetriever->Errors())
{
    $errorOutput .= $data;
}

$rv = $logRetriever->Close();
if ($rv)
{
    Header('Content-Type: text/html');
    print "An error occured while executing the command:<br /><pre>".htmlspecialchars($cmd)."</pre><br />The command reported a return value of $rv.";
    if (strlen($errorOutput))
    {
        print "Error message:<pre>$errorOutput</pre>\r\n";
    }
}
exit();

function startElement($parser, $name, $attrs)
{
    global $node, $logEntry, $url, $thisurl, $qs;
    $node = $name;
    switch ($node)
    {
        case "log":
        {
            echo '<?xml version="1.0" encoding="UTF-8"?>
            <feed xmlns="http://www.w3.org/2005/Atom">
            <title>'.htmlspecialchars(@$qs['title']).'</title>
            <subtitle>svn feed for '.htmlspecialchars($url).'</subtitle>
            <id>'.$thisurl.'</id>
            <link rel="self" href="'.$thisurl.'" />
            <updated>'.date('Y-m-d H:i:s.0T').'</updated>';
            break;
        }
        case "logentry":
        {
            // a new logentry
            $logEntry = array('rev' => @$attrs['revision'], 'allpaths' => array());
            break;
        }
        case "path":
        {
            $logEntry['action'] = @$attrs['action'];
            break;
        }

    }
}

function endElement($parser, $name)
{
    global $logEntry,$url,$thisurl,$qs;
    @$logEntry[$name] = trim(@$logEntry[$name]);
    switch ($name)
    {
        case "logentry":
        {
            // Create a title from the first line of the logentry msg
            $title = wordwrap(trim(@$logEntry['msg']));
            $tok = strtok($title, "\n");
            if (strlen($tok)<strlen($title))
            {
                $title = $tok."...";
            }
            // Cope with empty message by using the revision number in title
            if (strlen($title) == 0)
            {
                $title = 'Rev '.@$logEntry['rev'];
            }
            $msg = '<p><strong>Rev '.@$logEntry['rev'].' - '.@$logEntry['author'].'</strong></p><p>'.nl2br(@$logEntry['msg']).'</p>';
            if (array_key_exists('verbose', $qs))
            {
                $num = count($logEntry['allpaths']);
                $msg .= "<p>$num changed path(s):</p>";
                foreach ($logEntry['allpaths'] as $value)
                {
                    # Can't use pre tags for this block as some readers don't keep the newlines (e.g. RSSOwl)
                    # so use code and br.
                    $msg .= '&nbsp;&nbsp;&nbsp;&nbsp;<code>'.$value['action'].' '.$value['path']."</code><br />\r\n";
                }
            }            
            echo '<entry>
            <title>'.htmlspecialchars($title).'</title>
            <id>'.$thisurl.'?url='.urlencode(@$url).'&amp;rev='.@$logEntry['rev'].'</id>
            <link rel="alternate" href="'.$thisurl.'?url='.urlencode(@$url).'&amp;rev='.@$logEntry['rev'].'&amp;verbose=1" />
            <updated>'.htmlspecialchars(@$logEntry['date']).'</updated>
            <author><name>'.htmlspecialchars(@$logEntry['author']).'</name></author>
            <content type="html">'.htmlspecialchars($msg).'</content>
            </entry>';
            break;
        }
        case "log":
        {
            echo "</feed>";
            break;
        }
        case "path":
        {
            array_push($logEntry['allpaths'], array('action' => $logEntry['action'], 'path' => $logEntry['path']));
            $logEntry['path'] = '';
            break;
        }
    }
}

function characterData($parser, $data)
{
    global $node, $logEntry;
    @$logEntry[$node] .= $data;
}
?>
