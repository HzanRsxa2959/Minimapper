#include "plugin.h"
//>
#include <filesystem>
#include "include/IniReader-master/IniReader.h"
//<

using namespace plugin;
//>
using namespace std;
namespace fs = filesystem;

auto modfolder = fs::path(PLUGIN_PATH("")) / fs::path(PLUGIN_FILENAME).replace_extension("");
auto modini = CIniReader((modfolder / fs::path("config.ini")).string());

auto searchfolder = modfolder / fs::path("..radar");
auto linkfolder = modfolder / fs::path(".radar");
auto renamefolder = modfolder / fs::path("radar");

auto foldersDelete() {
	fs::remove_all(linkfolder);
	fs::remove_all(renamefolder);
}
//<

class Minimapper {
public:
    Minimapper() {
        // Initialise your plugin here
        
//>
		foldersDelete();

		fs::create_directories(searchfolder);
		fs::create_directories(linkfolder);

		Events::initRwEvent += [] {
			auto sizeini = modini.ReadInteger("", "size", 12);
			auto sizegame = (int *)0x573D41;
			auto nameformat = (const char *)0x866B98;

			auto radartile = int();
			auto sizeradar = int();
			auto tilerow = int(); auto tilecolumn = int();
			auto sizehalf = int();
			char filename[255];
			for (auto directoryentry : fs::directory_iterator(searchfolder)) {
				auto entrypath = directoryentry.path();
				if (!fs::is_directory(entrypath)) {
					if (sscanf(entrypath.stem().string().c_str(), "radar%d", &radartile) == 1) {
						if (sizeini != *sizegame) {
							sizeradar = sizeini;
							tilerow = radartile / sizeradar; tilecolumn = radartile % sizeradar;
							sizehalf = sizeradar / 2;

							tilerow -= sizehalf;
							tilecolumn -= sizehalf;

							sizeradar = *sizegame;
							sizehalf = sizeradar / 2;
							tilerow += sizehalf; tilecolumn += sizehalf;

							radartile = (tilerow * sizeradar) + tilecolumn;
						}

						sprintf(filename, nameformat, radartile); fs::create_hard_link(entrypath, linkfolder / fs::path(filename).replace_extension(entrypath.extension()));
					}
				}
			}

			fs::rename(linkfolder, renamefolder);

			Events::shutdownRwEvent += foldersDelete;
		};
//<
    }
//>
//<
} minimapper;

//>
//<