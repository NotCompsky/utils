// ==UserScript==
// @name TwitterLinks
// @namespace http://wiki.greasepot.net/examples
// @description Replace twitter short urls with expanded urls
// @match https://twitter.com/*
// ==/UserScript==

// $('a.twitter-timeline-link').each(function(item){  $(this).attr('href', $(this).data('expanded-url') )   });

function fix() {
    var links = document.querySelectorAll('a.twitter-timeline-link');
    for (var i = links.length - 1; i >= 0; i--) {
        var link = links[i];
        var new_link = link.getAttribute('data-expanded-url')
        link.setAttribute('href', new_link);
    };
}

document.addEventListener("onmousedown", fix);
