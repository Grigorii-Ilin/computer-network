
void encrypt(char* key, int lenKey, char* txt, int lenTxt){
	int keyPos = 0;

	for (int i = 0; i < lenTxt; i++){
		int keyValue = key[keyPos]-'A';

		int shift = txt[i]+ keyValue;
		if (shift>'Z') {
			shift -= 26;
		}

		txt[i] = (char)shift;

		keyPos++;
		if (keyPos>= lenKey){
			keyPos = 0;
		}
	}
}


void decrypt(char* key, int lenKey, char* txt, int lenTxt) {
	int keyPos = 0;

	for (int i = 0; i < lenTxt; i++) {
		int keyValue = key[keyPos] - 'A';

		int shift = txt[i] - keyValue;
		if (shift < 'A') {
			shift += 26;
		}

		txt[i] = (char)shift;

		keyPos++;
		if (keyPos >= lenKey) {
			keyPos = 0;
		}
	}
}