




#define CURL_STATICLIB
//#include<cJSON.h>;
#include<stdio.h>
#include<curl/curl.h>
#include<stdlib.h>
#include<string.h>




char storedtext[1000];
char* newptr = &storedtext;
size_t datasize;
size_t chucksize;



size_t writeData(char*ptr, size_t data, size_t numdata, void *userdata){
	chucksize = data * numdata;
	printf("%zu\n", chucksize);
	for (size_t a = datasize; a < strlen(ptr); a++){
		newptr[a] = ptr[a];
	}
	datasize += chucksize;
	return chucksize;
}

char* jsonParser(char*myJson,char*key,char * rtnVal) {
	boolean isKey = TRUE; 
	boolean isValue = FALSE;
	size_t len = strlen(myJson);
	int printLen = 0;
	int wait = 0;
	int braces = 0;
	char keyVal[100] = "";
	char myVal[256] = "";
	for (size_t a = 0; a < len; a++) {
		int wait = 0;
		// checking if the current character equals '{'
		if (myJson [a] == '{') braces++;

		// checking if the current character equals '}' 
		if (myJson[a] == '}') braces--;

		// checking for the next key.
		if (isKey && myJson[a] != '\"' && myJson[a] != ':' && myJson[a] != ',' && myJson[a] != '{' && myJson[a] != "" && myJson[a] != '}' && braces >0) {
			keyVal[printLen] = myJson[a];
			printLen++;
			//printf("myJson[a] = %c", myJson[a]);
		}
		
		// checking for the ':' character so as to stop adding  onto the myKey variable.
		if (isKey && myJson[a] == ':') {
			printLen = 0;
			isKey = FALSE;
			// checking if the mykey value equals the value of the key passed in as an argument.
			if (strcmp(key, keyVal) == 0) {
				isValue = TRUE;
				wait = 1;
			}
			memset(keyVal, 0, strlen(keyVal));
		}
		// cecking if it is time to read the nex key 
		if (!isValue && !isKey && myJson[a] == ',') isKey = TRUE;


		// checking if the current character is ,
		if (isValue && braces == 1 && myJson[a] == ',') {
			isValue = FALSE;
			a = len; 
		}
		// checking if isvalue is true snd writing to the myval varriable.
		if (isValue && wait == 0) {
			myVal[printLen] = myJson[a];
			printLen++;
		}
		
	}
	printf("\n\nmyvalue = %s", myVal);
	for (int a = 0; a<strlen(myVal); a++) rtnVal[a] = myVal[a];
}

int main(int a, char* argv[]) {

	//https://accelerexportal.app:8084/anp/api/v3/linuxpostest/login
	//char header[1000] = "content-type:application/json x-source-code:test x-client-id:test ";


	CURL* curl;
	curl = curl_easy_init();

	struct curl_slist* header = NULL;
	header = curl_slist_append(header, "content-type:application/json");
	header = curl_slist_append(header, "x-source-code:test");
	header = curl_slist_append(header, "x-client-id:test");

	curl_easy_setopt(curl, CURLOPT_URL, "https://accelerexportal.app:8084/anp/api/v3/linuxpostest/login");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"userReference\":\"test\",\"terminalId\":\"0000\",\"password\":\"test\",\"deviceId\":\"111111\"}");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, newptr);

	int resCode = curl_easy_perform(curl);
	char* prop = ",\"getProp\":\"balance\"}";

	for (int a = 0; a < strlen(prop); a++) {
		newptr[datasize + a -1] = prop[a];
	}

	
	printf("\nresponse code: %s\n", curl_easy_strerror(resCode));
	printf("\nrespose data:\n %s", newptr );
	printf("\n Total data received :%zu bytes \n\n\n", datasize);

	char* json = newptr;
	char key[50] = " ";
	char* mykey = &key;
	char value [256] =" ";
	char* myvalue = &value;
	boolean iskey = TRUE;
	boolean isvalue = FALSE;
	char* getprop = "balance";  
	size_t responseLen = strlen(json);
	size_t indexPos = 0;
	//printf("\nrespose length is:%zu\n", responseLen);
	
	for (size_t a = 0; a < responseLen; a++) { 
		//reading the next key in the stringifieed json
		if (iskey && json[a] != '\"' && json[a] != ':' && json[a] != ',' && json[a] != '{' && json[a] != "" && json[a] != '}'){
			mykey[indexPos] = json[a];
			printf("%c", mykey[indexPos]);
			indexPos++;
		}if (iskey && json[a] == ':') {
			iskey = FALSE;
			printf("\ncurrent value of mykey = %s", mykey);
			printf("\nkey len = %zu", strlen(mykey));
			printf("\nstrcmp = %d", strcmp(mykey,getprop));
			// checking if the key equals the key we want to check
			if (strcmp(mykey,getprop) == 0) {
				iskey = FALSE;
				isvalue = TRUE;
				printf("\n\n\niskey is false");
			}
			// emptying the value of the mykey variable
			memset(mykey, 0, strlen(mykey));
			printf("\nthe empty value of mykey = %s\n\n", mykey);
			indexPos = 0;
		}
		//setting to read the next key in the stringified json 
		if (!iskey && !isvalue && json[a] == ',') iskey = TRUE;
		
		// reading the value of.
		if (isvalue && json[a] != '\"' && json[a] != ':' && json[a] != ','  && json[a] != '}') {
			myvalue[indexPos] = json[a];
			indexPos++;
		}if (isvalue && json[a] == ',') {
			a = responseLen;
			printf("\nfinally checked for the value.");
		}

	}

	printf("\n\n\tbalance : %s\n\n", myvalue);

	char rtnVal[256] = "";
	jsonParser(json, &"balance",&rtnVal);
	printf("\n\nprinting the output of the json parser :\n %s", rtnVal);
	curl_easy_cleanup(curl);
	curl_slist_free_all(header);

	

	/**/
	//char* postme = "{\"name\" : \"hammed segun\", \"email\" : \"hammed@yahoo.com\", \"id\" : 400, \"getProp\" : \"name\"}";
	
	//printf("balance : ");
	//CURL* getdata;
	//getdata = curl_easy_init();
	//struct curl_slist* newheader = NULL;
	//newheader = curl_slist_append(newheader, "Content-Type:application/json");
	//newheader = curl_slist_append(newheader, "Accept:*/*");
	//curl_easy_setopt(getdata, CURLOPT_HTTPHEADER, newheader);
	//curl_easy_setopt(getdata, CURLOPT_URL, "http://localhost:4000/api/members/getdata");
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"name\" : \"hammed\", \"email\" : \"hammed@yahoo.com\", \"id\" : 200, \"getProp\" : \"name\"}");
	//curl_easy_setopt(curl, CURLOPT_POSTFIELDS,newptr);
	//int res = curl_easy_perform(getdata);
	//printf("\nrequest code: %s", curl_easy_strerror(res));
	//curl_easy_cleanup(getdata);

	//curl_slist_free_all(newheader);
	
	//printf("\n execution finished");
	 
	return 0;
}

// C program to show the use of puts
