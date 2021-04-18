import csv;
import sys;

from Levenshtein import distance as levenshtein;

class SoundEntry:
	def __init__(this, row):
		this.bank = row[0];
		this.soundName = row[1];
		this.programChangeSequence = row[2];
		this.soundSet = row[3];
		
	def __str__(this):
		return str([this.bank, this.soundName, this.programChangeSequence, this.soundSet]);
		
	def IsEqualMapping(this, other):
		return (this.programChangeSequence == other.programChangeSequence) and (this.soundSet == other.soundSet);
		
	def Key(this):
		return this.bank + "/" + this.soundName;

def LoadSoundMap(filePath):
	result = [];
	
	with open(filePath) as csvDataFile:
		csvReader = csv.reader(csvDataFile);
		
		firstLine = True;	
		for row in csvReader:
			if(firstLine): #column headers
				firstLine = False;
				continue;
				
			result.append( SoundEntry(row) );
				
	return result;
	
def BankPrefixMatch(p1, p2):
	#TODO
	return 0;
	
def BankDistance(b1, b2):
	if(b1 == b2):
		return 0;
		
	p1 = b1.split("/");
	p2 = b2.split("/");
	
	l1 = p1[-1];
	l2 = p2[-1];
	
	if(l1 == "Drum & SFX" and l2 == "User DK"):
		d = 1;
	elif(l2 == "User"):
		d = 1;
	else:
		d = levenshtein(l1, l2);
	
	return BankPrefixMatch(p1[0:-1], p2[0:-1]) + d;
	
def EntryDistance(e1, e2):
	d = BankDistance(e1.bank, e2.bank);
	d += levenshtein(entry.soundName, targetEntry.soundName);
	
	return d;
	
	
if(sys.argv[1] == "--skip-equal"):
	skipEqual = True;
	argIdx = 1;
else:
	skipEqual = False;
	argIdx = 0;

sourceFilePath = sys.argv[argIdx + 1];
targetFilePath = sys.argv[argIdx + 2];
maxD = int(sys.argv[argIdx + 3]);

sourceMap = LoadSoundMap(sourceFilePath);
targetMap = LoadSoundMap(targetFilePath);

mapping = {};
notMapped = [];
for entry in sourceMap:
	bestTargetRow = None;
	bestD = maxD + 1;
	for targetEntry in targetMap:
		d = EntryDistance(entry, targetEntry);
		if(d < bestD):
			bestD = d;
			bestTargetRow = targetEntry;
			
	if(bestTargetRow is None):
		notMapped.append(entry);
	else:
		mapping[entry] = bestTargetRow;
		
		
csvWriter = csv.writer(sys.stdout);
csvWriter.writerow(["Source set name", "Source sound name", "Source program change sequence", "Source sound set", "Target set name", "Target sound name", "Target program change sequence", "Target sound set"]);
for src in mapping:
	dest = mapping[src];
	
	if(skipEqual and src.IsEqualMapping(dest)):
		continue;
	
	csvWriter.writerow([src.bank, src.soundName, src. programChangeSequence, src.soundSet, dest.bank, dest.soundName, dest.programChangeSequence, dest.soundSet]);
	
for entry in notMapped:
	csvWriter.writerow([entry.bank, entry.soundName, entry.programChangeSequence, entry.soundSet, "", "", "", ""]);
