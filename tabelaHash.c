#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 101


typedef struct Contact {
    char name[50];
    char phone[20];
    struct Contact* next;
} Contact;


Contact* hashTable[TABLE_SIZE];


unsigned int hashFunction(const char* name) {
    unsigned int hash = 0;
    while (*name) {
        hash += *name++;
    }
    return hash % TABLE_SIZE;
}


void addContact(const char* name, const char* phone) {
    unsigned int index = hashFunction(name);
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    if (!newContact) {
        printf("Erro: Falha ao alocar memória.\n");
        return;
    }
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    newContact->next = hashTable[index];
    hashTable[index] = newContact;

    printf("Contato adicionado com sucesso.\n");
}


void searchContact(const char* name) {
    unsigned int index = hashFunction(name);
    Contact* current = hashTable[index];

    while (current) {
        if (strcmp(current->name, name) == 0) {
            printf("Telefone de %s: %s\n", name, current->phone);
            return;
        }
        current = current->next;
    }
    printf("Erro: Contato não encontrado.\n");
}


void removeContact(const char* name) {
    unsigned int index = hashFunction(name);
    Contact* current = hashTable[index];
    Contact* previous = NULL;

    while (current) {
        if (strcmp(current->name, name) == 0) {
            if (previous == NULL) {
                hashTable[index] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            printf("Contato removido com sucesso.\n");
            return;
        }
        previous = current;
        current = current->next;
    }
    printf("Erro: Contato não encontrado.\n");
}


void displayContacts() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        if (current) {
            printf("Índice %d:\n", i);
            while (current) {
                printf("  Nome: %s, Telefone: %s\n", current->name, current->phone);
                current = current->next;
            }
        }
    }
}


void freeMemory() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* current = hashTable[i];
        while (current) {
            Contact* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}


double measureTime(void (*func)(const char*, const char*), const char* name, const char* phone) {
    clock_t start, end;
    start = clock();
    func(name, phone);
    end = clock();
    return ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
}


int main() {
    int option;
    char name[50], phone[20];


    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    do {
        printf("\nEscolha uma opção:\n");
        printf("1 - Adicionar contato\n");
        printf("2 - Buscar contato por nome\n");
        printf("3 - Remover contato\n");
        printf("4 - Exibir todos os contatos\n");
        printf("0 - Sair\n");
        printf("Digite uma opção: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                printf("Nome: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                printf("Telefone: ");
                fgets(phone, sizeof(phone), stdin);
                phone[strcspn(phone, "\n")] = '\0';

                printf("Tempo de inserção: %.2f ms\n",
                       measureTime((void (*)(const char*, const char*))addContact, name, phone));
                break;
            case 2:
                printf("Nome: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                printf("Tempo de busca: %.2f ms\n",
                       measureTime((void (*)(const char*, const char*))searchContact, name, ""));
                break;
            case 3:
                printf("Nome: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                removeContact(name);
                break;
            case 4:
                displayContacts();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (option != 0);

    freeMemory();

    return 0;
}
