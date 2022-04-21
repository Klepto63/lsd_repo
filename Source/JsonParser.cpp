#include "JsonParser.h"
#include "Path.h"

var jsonFile;
int librairy_size;


int jsonParserLoad(int idx, s_metadata* metadata)
{

	if (idx < librairy_size) //todo checker si on a trouve les data dans le json
	{
		juce::String pathh  = PathGetAsset(PATH_SONG) +  jsonFile["library"][idx]["folder"];
		metadata->folder	= jsonFile["library"][idx]["folder"];
		metadata->id		= jsonFile["library"][idx]["id"];
		metadata->composer  = jsonFile["library"][idx]["composer"];
		metadata->title		= jsonFile["library"][idx]["title"];
		metadata->pathImage = pathh + jsonFile["library"][idx]["image"];
		metadata->pathSong  = pathh + jsonFile["library"][idx]["path"];
		metadata->stem_nb	= jsonFile["library"][idx]["stem_nb"];
		
		for (int i = 0; i < metadata->stem_nb; i++)
		{
			metadata->stem[i].name = pathh + jsonFile["library"][idx]["stems"][i]["name"];
			metadata->stem[i].path = pathh + jsonFile["library"][idx]["stems"][i]["path"];
		}

		return 0;
	}

	return 1; //not found
 }

int jsonParserGetNbSong()
{
	return librairy_size;
}

void jsonParserInit(void)
{
	jsonFile = JSON::parse(File(PathGetAsset(PATH_SONG) +  (juce::String) "song.json") );
	librairy_size = jsonFile["librairy-size"];
}

