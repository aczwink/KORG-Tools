import csv;
import sys;

from Levenshtein import distance as dist_func;

def LoadSoundMap(filePath):
	result = {};
	
	with open(filePath) as csvDataFile:
		csvReader = csv.reader(csvDataFile);
		
		firstLine = True;	
		for row in csvReader:
			if(firstLine): #column headers
				firstLine = False;
				continue;
				
			key = row[0];
			if(key in result):
				raise NotImplementedError("DUPLICATE SOUND: " + key);
			result[key] = ( row[1], row[2] );
				
	return result;

sourceFilePath = sys.argv[1];
targetFilePath = sys.argv[2];
maxD = int(sys.argv[3]);

sourceMap = LoadSoundMap(sourceFilePath);
targetMap = LoadSoundMap(targetFilePath);

mapping = {};
notMapped = [];
for key in sourceMap:
	if(key in targetMap):
		mapping[key] = (key, targetMap[key]);
	else:
		bestKey = None;
		bestD = maxD;
		for k2 in targetMap:
			d = dist_func(key, k2);
			if(d < bestD):
				bestD = d;
				bestKey = k2;
				
		if(bestKey is None):
			notMapped.append(key);
		else:
			mapping[key] = (bestKey, targetMap[bestKey]);
		
		
csvWriter = csv.writer(sys.stdout);
csvWriter.writerow(["Source sound name", "Source program change sequence", "Source sound set", "Target sound name", "Target program change sequence", "Target sound set"]);
for key in mapping:
	src = sourceMap[key];
	dest = mapping[key];
	csvWriter.writerow([key, src[0], src[1], dest[0], dest[1][0], dest[1][1]]);
	
for key in notMapped:
	src = sourceMap[key];
	csvWriter.writerow([key, src[0], src[1], "", "", ""]);
