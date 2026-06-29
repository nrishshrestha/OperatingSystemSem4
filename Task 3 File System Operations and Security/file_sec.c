#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

// creating a audit log
void audit_log(const char* action) {
	FILE *log_file = fopen("audit.log","a");
	if(log_file) {
		fprintf(log_file, "[AUDIT] %s\n", action);
		fclose(log_file);
	}
}

// Encrypting and Decrypting using a simple XOR function
void encrypt_decrypt(char *data, char key) {
	for(int i = 0; i < strlen(data); i++) {
		data[i] = data[i] ^ key; //XOR flips the bits for encryption, repeating this twice decrypts it
	}
}

// User Authentication
int authenticate() {
	char password[20];
	printf("Enter System Password to Continue: "); // Fixed typo: prinf -> printf
	scanf("%19s", password); // Fixed typo: scabf -> scanf
	
	if(strcmp(password, "admin123") == 0) {
		audit_log("USER SUCCESSFULLY AUTHENTICATED!");
		return 1;
	}
	audit_log("FAILED AUTHENTICATION ATTEMPT!");
	return 0;
}

int main () {
	if(!authenticate()) {
		printf("Access Denied.\n");
		return 1;
	}
	printf("Authentication Successful.\n");

	// creating files and assigning permission for specific users
	FILE *f1 = fopen("user1_readonly.txt","w");
	FILE *f2 = fopen("user2_readwrite.txt","w");
	FILE *f3 = fopen("user3_execute.txt","w");

	if(f1) { fprintf(f1, "Data for User 1 \n"); fclose(f1); } // Fixed typo: fcolse -> fclose
	if(f2) { fprintf(f2, "Data for USer 2 \n"); fclose(f2); }
	if(f3) { fprintf(f3, "Binary placeholder \n"); fclose(f3); }
	audit_log("Created 3 base files for users.");

	// Applying linux permission using chmod()
	chmod("user1_readonly.txt", S_IRUSR); // Read only (0400)
	chmod("user2_readwrite.txt", S_IRUSR | S_IWUSR); // Read + Write (0600)
	chmod("user3_execute.txt", S_IXUSR); // Execute only (0100)
	audit_log("Applied strict chmod permissions to all 3 files.");

	printf("Permission Test \n");

	//Test 1 Testing if you can write to the read only file
	FILE *test1 = fopen("user1_readonly.txt","w");
	if(test1 == NULL) {
		printf("User 1 Write Test: Failed. (expected)\n Justification: File is strictly set to Read only. The OS blocked modification.\n");
		audit_log("Blocked unauthoried write attempt on user1_readonly.txt");
	} else {
		fclose(test1);
	}

	//Test 2 Testing to write to the read + write file
	FILE *test2 = fopen("user2_readwrite.txt","a");
	if(test2 != NULL) {
		fprintf(test2, "Appending new data. \n");
		printf("User 2 Write Test: Success. (expected)\n Justification: File has Write permission.\n");
		fclose(test2);
		audit_log("Successful write to user2_readwrite.txt");
	}

	// Encryption / Decryption Demonstration
	printf("ENCRYPTING...");
	char sensitive_data[] = "Confidential Do Not Share";
	char key = 'K'; // Fixed type mismatch: changed "K" (string) to 'K' (char)

	printf("Original Data: %s\n", sensitive_data);

	encrypt_decrypt(sensitive_data, key);
	printf("Encrypted Data: %s \n", sensitive_data); // Fixed typo: prinf -> printf
	audit_log("Encrypted sensitive data in memory.");
	
	encrypt_decrypt(sensitive_data, key);
	printf("Decrypted Data: %s\n", sensitive_data);
	audit_log("Decrypt sensitive data in memory.");

	return 0;
}
