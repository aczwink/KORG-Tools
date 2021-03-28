import csv;
import sys;
import xml.etree.ElementTree as ET

inputFilePath = sys.argv[1];

root = ET.Element("SoundMap");
currentSoundSetNode = None;
bankCounter = None;

with open(inputFilePath) as csvDataFile:
	csvReader = csv.reader(csvDataFile);
	
	firstLine = True;	
	for row in csvReader:
		if(firstLine): #column headers
			firstLine = False;
			continue;
			
		if(not(row[1]) and not(row[2]) and not(row[3])):
			parts = row[0].split("/");
			
			soundSetName = parts[0];
			del parts[0];
			if(soundSetName == "GM"):
				soundSetName += "/" + parts[0];
				del parts[0];
			
			if((currentSoundSetNode is None) or not(currentSoundSetNode.attrib["name"] == soundSetName)):
				attribs = {"name": soundSetName};
				currentSoundSetNode = ET.Element("SoundSet", attribs);
				root.append(currentSoundSetNode);
				bankCounter = 1;
				
			if(len(parts) == 0):
				continue;
				
			assert len(parts) == 1, row;
			attribs = {
				"name": parts[0],
				"bankNumber": str(bankCounter),
			};
			bankCounter += 1;
			currentBankNode = ET.Element("Bank", attribs)		
			currentSoundSetNode.append(currentBankNode);
		else:
			attribs = {
				"name": row[0],
				"bankSelectMSB": row[1],
				"bankSelectLSB": row[2],
				"programChange": row[3],
			};
			currentSoundNode = ET.Element("SoundEntry", attribs);
			currentBankNode.append(currentSoundNode);

tree = ET.ElementTree(root);
tree.write(sys.stdout, encoding="unicode");
