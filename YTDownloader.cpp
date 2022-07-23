#include "src/YTDownloader.h"

int main() {
	install();
	
	std::string yt_url("https://www.youtube.com/watch?v=ph2ZBNFzhsQ");
	HtmlOnMemory html = get_html_doc(yt_url);
	//std::cout << html.html << std::endl;
	Video json = get_video_from_html(html.html);
	json.print_details();

	json.get_video_format_list();
	std::string video_quality = "tiny";
	//std::cin >> video_quality;
	json.download("video", video_quality);
	//std::cin.get();

	json.get_audio_format_list();
	std::string audio_quality = "tiny";
	//std::cin >> audio_quality;
	json.download("audio", audio_quality);
	//std::cin.get();
}