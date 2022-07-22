#include "src/YTDownloader.h"

int main() {
	install();
	
	std::string yt_url("https://www.youtube.com/watch?v=u31qwQUeGuM");
	HtmlOnMemory html = get_html_doc(yt_url);
	//std::cout << html.html << std::endl;
	Video json = get_video_from_html(html.html);
	json.print_details();
}