// Your code here!
var tracker;

function parse( hit ) 
{
    var pair;
    var result = "";

    var iter = hit.data.first();

    while (iter.hasCurrent)
    {
        result += iter.current.key ;
        result += ":";
        result += iter.current.value;  
        result += "\n";
        iter.moveNext(); 
    }

    return result; 

}

function showResult (  hit, msg ) 
{
    document.getElementById('textResponse').value = msg;
    document.getElementById('textRequest').value = parse(hit); 
}


function onHitMalformed ( args ) 
{
    showResult(args.hit, "**hit malformed** \n" + args.httpStatusCode); 
}

function onHitFailed ( args ) 
{
    showResult(args.hit, "**hit failed**\n" +  args.error.message); 
}

 
function onHitSent ( args ) 
{
    showResult(args.hit, args.response);  
}
function initTracker() {

    tracker = GoogleAnalytics.AnalyticsManager.current.createTracker("UA-39959863-1");     
    GoogleAnalytics.AnalyticsManager.current.isDebug = true;
    GoogleAnalytics.AnalyticsManager.current.reportUncaughtExceptions = true;
    GoogleAnalytics.AnalyticsManager.current.autoAppLifetimeMonitoring = true;

     
}

function sendEvent  () 
{
    tracker.send(GoogleAnalytics.HitBuilder.createCustomEvent("test", "userclick", "", 0).build()); 
}

function sendPageview() {
     
    tracker.send(GoogleAnalytics.HitBuilder.createScreenView( "index.html").build());
     
}

function sendSocial() {
    tracker.send(GoogleAnalytics.HitBuilder.createSocialInteraction("facebook", "share", "index.html" ).build());
}

function sendTiming () 
{
    tracker.send(GoogleAnalytics.HitBuilder.createTiming("pageload", "index.html", 10, "").build()); 
}

function sendException () 
{
    tracker.send(GoogleAnalytics.HitBuilder.createException("Oops. Something bad happened", false).build()); 
}


function addListeners () 
{
    document.getElementById('btnSendEvent').addEventListener("click", sendEvent, false );
    document.getElementById('btnSendPageView').addEventListener ("click", sendPageview, false ); 
    document.getElementById('btnSendSocial').addEventListener ("click", sendSocial , false ); 
    document.getElementById('btnSendTiming').addEventListener ("click", sendTiming, false ); 
    document.getElementById('btnSendException').addEventListener ("click", sendException, false );
     
  
    GoogleAnalytics.AnalyticsManager.current.addEventListener("hitfailed", onHitFailed, false );
    GoogleAnalytics.AnalyticsManager.current.addEventListener("hitsent", onHitSent , false );    
    GoogleAnalytics.AnalyticsManager.current.addEventListener("hitmailformed", onHitMalformed, false ); 
     

}



function init() 
{
    initTracker();
    addListeners();

}


document.onload = init();


