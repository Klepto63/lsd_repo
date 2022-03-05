#include "JsonParser.h"
#include "Path.h"

var jsonFile;
int librairy_size;


int jsonParserLoad(int idx, s_metadata* metadata)
{
	if (idx < librairy_size) //todo checker si on a trouve les data dans le json
	{
		metadata->id		= jsonFile["library"][idx]["id"];
		metadata->composer  = jsonFile["library"][idx]["composer"];
		metadata->title		= jsonFile["library"][idx]["title"];
		metadata->pathImage = jsonFile["library"][idx]["image"];
		metadata->pathSong  = jsonFile["library"][idx]["path"];
		metadata->stem_nb	= jsonFile["library"][idx]["stem_nb"];

		for (int i = 0; i < metadata->stem_nb; i++)
		{
			metadata->stem[i].name = jsonFile["library"][idx]["stems"][i]["name"];
			metadata->stem[i].path = jsonFile["library"][idx]["stems"][i]["path"];
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
	jsonFile = JSON::parse(File(ABS_PATH_SONGS + (juce::String) "song.json"));
	librairy_size = jsonFile["librairy-size"];

}

