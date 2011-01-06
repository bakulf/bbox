<?php
require('lessphp/lessc.inc.php');

$less = new lessc('css/style.less');
file_put_contents('css/style.css', $less->parse());
$bbox_version_linux = '0.1';
$bbox_version_mac = '0.1-alpha';
$bbox_version_win = '0.1-beta';
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta name="keywords" content="BBOX, version, control, tool, cloud, data, share, sharing" />
<meta name="description" content="BBOX is your (version) control tool for your cloud data." />
<title>BBOX - The cloud under your  (version) control, as easy and drag 'n drop.</title>
<link rel="stylesheet" href="framework/reset.css" />
<link rel="stylesheet" href="framework/text.css" />
<link rel="stylesheet" href="framework/960.css" />
<link href="css/style.css" rel="stylesheet" media="screen">
<link href='http://fonts.googleapis.com/css?family=Arvo' rel='stylesheet' type='text/css'>
<script src="http://code.jquery.com/jquery-1.4.4.min.js"></script>
<script src="http://cdn.jquerytools.org/1.2.5/jquery.tools.min.js"></script>
<script src="http://remysharp.com/downloads/html5.js"></script>
<script src="js/ticker.js"></script>
<script src="js/jquery.zrssfeed.min.js"></script>
<script type="text/javascript" src="http://use.typekit.com/ojo1ipy.js"></script>
<script type="text/javascript">try{Typekit.load();}catch(e){}</script>
<script type="text/javascript">
$(
function() {
  $('#commits').rssfeed('https://github.com/bakulf/bbox/commits/master.atom', {limit:3})
  $('.package').live('click', function() {
                                $('.download').attr('href', '/package/' + $(this).attr('alt'));
                              });
});
</script>
</head>

<body>
<div class="container_12 clearfix">
  <header class="grid_12">
    <section id="logo" class="push_4 grid_3">
      <a href="/" title="BBOX - Home page" class="home">BBOX - Home page</a>
    </section>
    <section id="claim" class="grid_12">
      <h1>the cloud under your <em>(version)</em> control.</h1>
    </section>
  </header>
  <section id="highlight" class="grid_12">
    <section id="screencast" class="grid_6">
      <h2>As easy as drag ‘n drop.</h2>
      <video><a href="#" class="play">play</a></video>
      <h3>plus, you can control this.</h3>
    </section>
    <section id="download" class="push_2 grid_4">
      <h2>download</h2>
      <a href="" title="download" class="download">
        download the app
      </a>
      <form action="" method="get" id="platform" class="grid_4">
        <legend id="platform_legend">choose your platform <br /> and start using it</legend>
        <fieldset>
          <label for="linux" id="gnulinux" title="Debian Package (v. <?php print $bbox_version_linux?>)">GNU/Linux</label>
          <input type="radio" alt="bbox-<?php print $bbox_version?>.deb" name="platform" id="linux" class="package" />
        </fieldset>
        <fieldset>
          <label for="macosx" id="mac" title="Mac OS X DMG (v. <?php print $bbox_version_mac?>)">Mac OS X 10.5.x</label>
          <input type="radio" alt="bbox-<?php print $bbox_version?>.dmg" name="platform" id="macosx" class="package" />
        </fieldset>
        <fieldset>
          <label for="windows" id="win" title="Windows Executable (v. <?php print $bbox_version_win?>)">Windows</label>
          <input type="radio" alt="bbox-<?php print $bbox_version?>.exe" name="platform" id="windows" class="package" />
        </fieldset>
      </form>
    </section>
    <div id="footer_highlight">&nbsp;</div>
  </section>
  <section id="main" class="grid_12">
    <section id="center" class="alpha grid_7">
      <h2>No more quotas (unless you want them).</h2>
      <p>You are in control of your Cloud. Because BBox is based on <a href="http://subversion.tigris.org">Subversion</a> technology, so you install the server, you choose the settings. You choose whether to encrypt the main storage hard drive or not. If using a Secure protocol like <acronym title="Hyper Text Transfer Protocol Secured">HTTPS</acronym> or not.</p>
      <h3>Just work(s)</h3>
      <h4>What if your work is shared to your team, seamlessly?</h4>
      <p>BBox leaves you alone. It hardly bothers you with notifications. It lets you work and, transparently to you, it just shares your folders and documents.<br />
      This happens through your server, under your terms, your rules, and <a href="">group permissions</a> you can decide.
      </p>
      <ol>
        <li>Setup a shared folder</li>
        <li>Start sharing...</li>
        <li>I'm sorry there's no three here...</li>
      </ol>
      <h3 class="snap">Oh snap!</h3>
      <h4>Wrong modification on file, am I lost?</h4>
      <p>Nope, you're not. BBox supports revision, fully. So you can <em>always</em> come back to a specific revision of the file, without losing anything, anytime.</p>
       <div class="face"><img src="images/happy-face.jpg" alt="sad face" /></div>
     <h5>Hey I have this beautiful idea on how to improve BBox...</h5>
      <p>Guess what? It's on <a href="http://github.com/bakulf/bbox">github</a>, fork it and give it back to the open source community.</p>
    </section>
    
    <aside id="service" class="grid_5 omega">
      <section id="twitter" class="ticker">
        <h3>our tweets</h3>
        <div id="tweets"></div>
      </section>
      <section id="github">
        <h3>our latest commits</h3>
        <section id="commits"></section>
      </section>
    </section>
  </aside>
</div>
<footer>
  <section class="container_12">
    <div class="grid_12 shadow">
      <h3>
        ... and it’s free! As in freedom (but we love free beers).
        <form action="https://www.paypal.com/cgi-bin/webscr" method="post" name="paypal" class="paypal"><input type="hidden" name="cmd" value="_donations"><input type="hidden" name="business" value="info@nois3lab.it"><input type="hidden" name="lc" value="IT"><input type="hidden" name="item_name" value="donate for a bbox beer"><input type="hidden" name="item_number" value="donazione"><input type="hidden" name="no_note" value="0"><input type="hidden" name="currency_code" value="EUR"><input type="hidden" name="bn" value="PP-DonationsBF:paypal_donate.jpg:NonHostedGuest"><input type="image" border="0" src="images/paypal.png" name="submit" alt="PayPal - Il sistema di pagamento online più facile e sicuro!"><img width="1" height="1" border="0" alt="" src="https://www.paypal.com/it_IT/i/scr/pixel.gif">
          </form>
      </h3>
      <h4><a href="http://nois3lab.it" title="nois3lab (cc) 2001"><img src="images/n3.png" /></a></h4>
    </div>
  </section>
</footer>
</body>
</html>
