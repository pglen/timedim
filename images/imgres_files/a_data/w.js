function st_go(a){var i,u=document.location.protocol+'//stats.wordpress.com/g.gif?host='+escape(document.location.host)+'&rand='+Math.random();for(i in a){u=u+'&'+i+'='+escape(a[i]);}u=u+'&ref='+escape(document.referrer);document.open();document.write("<img id=\"wpstats\" src=\""+u+"\" alt=\"\" />");document.close();}
function ex_go(a){var i,u=document.location.protocol+'//stats.wordpress.com/g.gif?v=wpcom2&rand='+Math.random();for(i in a){u=u+'&'+i+'='+escape(a[i]);}document.open();document.write("<img id=\"wpstats2\" src=\""+u+"\" alt=\"\" style=\"display:none\" />");document.close();}
function fzd(){
var pr=document.location.protocol||'https:';
if ( _error ) {
	var c = escape(_error);
	url=pr+"//stats.wordpress.com/e.gif?c="+c+"&rand="+Math.random();
} else if ( _admin ) {
	var p = escape(_page);
	url=pr+"//stats.wordpress.com/h.gif?admin=1&page="+p+"&rand="+Math.random();
} else {
	var t = escape(document.title);
	var b = escape(_blog);

	if ( typeof _subd != 'undefined' )
		var subd = '&subd=' + escape(_subd);
	else
		var subd = '';

	if ( typeof _site != 'undefined' )
		var s = '&sid=' + escape(_site);
	else
		var s = '';

	if ( typeof _post != 'undefined' )
		var p = escape(_post);
	else
		var p = 0;

	var f = "";
	if (typeof(pp_frames) != "undefined")
	 if (pp_frames)
	  f = top.document.referrer;
	if ((f == "") || (f == "[unknown origin]") || (f == "unknown") || (f == "undefined"))
	 if (document["parent"] != null)
	  if (parent["document"] != null) // ACCESS ERROR HERE!
	   if (parent.document["referrer"] != null)
	    if (typeof(parent.document) == "object")
	     f = parent.document.referrer;
	if ((f == "") || (f == "[unknown origin]") || (f == "unknown") || (f == "undefined"))
	 if (document["referrer"] != null)
	  f = document.referrer;
	if ((f == "") || (f == "[unknown origin]") || (f == "unknown") || (f == "undefined"))
	 f = "";
	f = escape(f);

	u = escape(document.location);

	if ( 'undefined' != typeof _admin )
	 a = "&admin=" + _admin;
	else
	 a = "";

	url=pr+"//stats.wordpress.com/i.gif?blog="+b+subd+a+"&post="+p+"&ref="+f+"&url="+u+"&user="+_user+"&rand="+Math.random()+s;
}
document.open();
document.write("<img id=\"wpstats\" src=\""+url+"\" alt=\"\" />");
document.close();
}

/*	The following Javascript includes code from
 *		http://verens.com/archives/2005/03/21/tracking-external-links-with-ajax/
 *	and
 *		http://www.xml.com/pub/a/2005/02/09/xml-http-request.html
 *	and
 *		http://andy.wordpress.com/
 */

function addEvent(el,ev,fn){
	var isIE=window.attachEvent?true:false;
	if(isIE)el.attachEvent('on'+ev,fn);
	else if(el.addEventListener)el.addEventListener(ev,fn,false);
}
function linkmousedown(event) {
	var isIE=window.attachEvent?true:false;
	event=event?event:(window.event?window.event:"");
	var m=isIE?window.event.srcElement:event.currentTarget;
	m.modo = true;
}
function linkmouseout(event) {
	var isIE=window.attachEvent?true:false;
	event=event?event:(window.event?window.event:"");
	var m=isIE?window.event.srcElement:event.currentTarget;
	m.modo = false;
}
function linkmouseup(event) {
	var isIE=window.attachEvent?true:false;
	event=event?event:(window.event?window.event:"");
	var m=isIE?window.event.srcElement:event.currentTarget;
	if (m.modo) linktracker_record(event);
}
function linkclick(event) {
	var isIE=window.attachEvent?true:false;
	event=event?event:(window.event?window.event:"");
	linktracker_record(event);
}
function linktracker_init(b,p){
	_blog = b;
	_post = p;
	if ( typeof document.location.host != 'undefined' )
		var localserver=document.location.host;
	else
		var localserver=document.location.toString().replace(/^[^\/]*\/+([^\/]*)(\/.*)?/,'$1');
	var els=document.getElementsByTagName('a');
	for(var i=0;i<els.length;i++){
		var href=els[i].href;
		if(href.match(eval('/^(http(s)?:\\/\\/)?'+localserver+'/'))) continue;
		addEvent(els[i],'mousedown',linkmousedown);
		addEvent(els[i],'mouseout',linkmouseout);
		addEvent(els[i],'mouseup',linkmouseup);
	//	addEvent(els[i],'click',linkclick);
	//	els[i].title = els[i].title + '(cmc)';
	}
}
function linktracker_record(event){
	var isIE=window.attachEvent?true:false;
	event=event?event:(window.event?window.event:"");
	var b=isIE?window.event.srcElement:event.currentTarget;
	while (b.nodeName != "A") {
		if ( typeof b.parentNode == 'undefined' ) return;
		b = b.parentNode;
	}
	var bh=b.href;
	//bh=bh.replace('://','/:/');
	var pr=document.location.protocol||'http:';
	var b=(typeof _blog != 'undefined')?_blog:0;
	var p=(typeof _post != 'undefined')?_post:0;
	var i=new Image(1,1);
	i.src=pr+'//stats.wordpress.com/c.gif?b='+b+'&p='+p+'&u='+escape(bh);
	i.onLoad=function(){cmcVoid();}
}
function cmcVoid() {return;}
