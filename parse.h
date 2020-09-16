#pragma once



/// <summary>
/// Initializes the vector of randomization settings and text. Loads in previous data of viewed items by the user. Throws errors if files are unable to be opened.
/// 
/// Requires log.txt to be preloaded with randomizer values.
/// </summary>
void initArr();


/// <summary>
/// After initializing the randomization settings into a vector, loads the section headers for each setting.
/// </summary>
void sectionTitles();


/// <summary>
/// Refreshes after viewed data has changed.
/// </summary>
void reloadViewed();

