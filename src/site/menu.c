#include <stdlib.h>
#include "site.h"
#include "decorator.h"
#include "files.h"

void headCallBack () {
	// TODO: return value
	(void)includeFile("analyticstracking.inc");
}

int main () {
	// assign file to standard out TODO: change to parameter
	cgi_out = stdout;

	cgiHeader(&headCallBack);
    cgiTag(tt_DIV, "entry-content", NULL);

    cgiSection("Florian Hofer", "home");

    cgiTag(tt_DIV, NULL);

    cgiTag(tt_DIV, NULL);
    cgiTag(tt_DIV, "", "", "display: inline; margin: 5px 10px; float: right;");
    cgiTag(tt_SPAN, "font-family: arial,sans-serif;");
    cgiTag(tt_SPAN, "font-size: medium;");
    cgiTag(tt_IMG, "../images/portrait.png", "195", "247"," class=\"\" border=\"0\"");
    cgiTagClose(tt_DIV);

    cgiTag(tt_P);
    cgiTag(tt_SPAN, "font-size: medium;");cgiOut("<!--Florian Hofer-->"); BR
    cgiOutI("Freelancer and PostDoc Researcher"); BR
    AFULL("Free University of Bolzano", "https://www.unibz.it", NULL); BR
	AFULL("University of California", "https://www.berkeley.edu", NULL); BR
	cgiOutI("POS 1.14"); BR
	cgiOutI("Faculty of Computer Science ("); AFULL("map", "https://goo.gl/maps/BDccUeNKTtdwd7kn7", NULL); cgiOut(")"); BR
	cgiOutI("Bolzano"); BR
	cgiOutI("IT"); BR
	cgiOutI("+39 0471 016029"); BR
	cgiOutI("florian.hofer@unibz.it ("); AFULL("PGP", "../docmisc/public.pgp", NULL); cgiOut(")");
    cgiTagClose(tt_DIV);

    cgiTag(tt_P);
    cgiTag(tt_SPAN, "font-size: medium;"); cgiTag(tt_B, NULL); cgiOut("About");
    cgiTagClose(tt_P);
    cgiTag(tt_P);
    cgiOutI("Florian Hofer is a Freelancer for Industrial automation and Industry 4.0, and a PostDoc Researcher. He was previously a Visiting Researcher at the University of California, Berkeley, where he collaborated with Siemens USA on a Real-Time Control Container project.");
    cgiTagClose(tt_P);
    cgiTagClose(tt_DIV);

//    <div>
//    <p>He was previously assistant professor at the <a href="https://www.unibz.it/">Free University of Bozen Bolzano</a>¹ (2014-2017). He holds a PhD from the Free University of Bozen-Bolzano (2014), and a Diplom (MSc.) from <a href="https://tu-dresden.de/?set_language=en">TU Dresden</a> (2010; <a href="wp-content/uploads/dresden.jpg">not this Dresden</a>). He spent time as visitor at the Max-Planck Institute for Informatics (2016), the <a href="http://www.uq.edu.au/">University of Queensland</a> (2015), <a href="http://www.research.att.com/">AT&amp;T Labs-Research</a> (2013), the <a href="https://ucsd.edu/">University of California, San Diego</a> (2012), and has previous industrial experience from <a href="http://www.globalfoundries.com/">Globalfoundries </a>(2010) and <a href="https://www.siemens.com/">Siemens IT</a> (2009). He has published 16 papers at premier² conferences in the area of data science and management (and more than 44 papers in total).</p>
//    <p style="text-align: right;"><a href="https://scholar.google.de/citations?user=ymKWDvoAAAAJ&amp;hl=en"><img class="alignnone size-full wp-image-154" src="wp-content/uploads/scholar.png" alt="" width="78" height="30"></a> <a href="http://dblp.uni-trier.de/pers/hd/r/Razniewski:Simon"><img class="alignnone size-full wp-image-153" src="wp-content/uploads/dblp.png" alt="" width="61" height="30"></a><a href="https://tools.wmflabs.org/reasonator/?&amp;q=51903121"><img class="alignnone wp-image-180" src="wp-content/uploads/wd-300x105.jpg" alt="" width="89" height="31" srcset="wp-content/uploads/wd-300x105.jpg 300w, wp-content/uploads/wd.jpg 426w" sizes="(max-width: 89px) 100vw, 89px"></a></p>
//    </div>

    cgiSection("News", "news");

    listNews();

    cgiSection("Publications", "pubs");

    listFiles();

    cgiSection("Projects", "projects");

    listProjects();

    cgiSection("Locations", "locations");

    listLocations();

    cgiTagClose(tt_DIV);

    cgiFooter(NULL);

    return EXIT_SUCCESS;
}
