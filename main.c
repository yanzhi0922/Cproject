#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_STUDENTS 100000

// ѧҵ�ɼ��ṹ��
typedef struct AcademicScoreNode {
    char courseName[MAX_NAME_LENGTH];
    float score;    // ����
    float credit; // ѧ��
    float gpa; // ���ſγ̼���
    struct AcademicScoreNode *next;
} AcademicScoreNode;

// ��ѧ�����´�ҵ�ƻ���Ŀ�ṹ��
typedef struct InnovationProject {
    char projectName[MAX_NAME_LENGTH]; // ��Ŀ����
    float gpa; // �ӷ�ֵ
    int isLeaderOrSecondLeader;// �Ƿ�Ϊ��Ŀ������,�Ƿ�Ϊ�ڶ������ˣ����Ҽ����������Ŀ�����˼�0.1 GPA���ڶ�����0.05 GPA
    char projectLevel[10];// ��Ŀ���𣬹��Ҽ���ʡ����У��
    int isFinished;    // �Ƿ����
} InnovationProject;

// ѧ�����Ľṹ��
typedef struct AcademicPaper {
    char title[MAX_NAME_LENGTH]; // ���ı���
    char journalName[MAX_NAME_LENGTH]; // �ڿ�����
    int isFirstAuthor; // �Ƿ�Ϊ��һ����

    //�ڿ����𣬶�����0.4��һ���ڿ���0.2 GPA�������ڿ���0.1 GPA�������ڿ���0.05 GPA���ļ��ڿ���0.02 GPA���弶�ڿ���0.01 GPA
    int journalLevel;

    float gpa; // �ӷ�ֵ
    struct AcademicPaper *next;
} AcademicPaper;

// �������ѧ�ƾ����ṹ��
typedef struct Competition {
    char competitionName[MAX_NAME_LENGTH]; // ��������
    int teamSize;   //������������
    char competitionLevel[10]; // ��������,���Ҽ���ʡ����У��
    int rank;   // �񽱵ȼ���1��ʾһ�Ƚ���2��ʾ���Ƚ���3��ʾ���Ƚ�
    char competitionType;   //�������A�࣬B�࣬C��
    float gpa; // �ӷ�ֵ
    struct Competition *next;
} Competition;

// ѧ����Ϣ�ṹ��
typedef struct StudentInfo {
    char studentID[8];// ѧ��
    char name[15];// ����
    char password[15];// ����
    int classNumber;// �༶
    AcademicScoreNode *academicScores;// ѧҵ�ɼ�
    InnovationProject *innovationProjects;// ��ѧ�����´�ҵ�ƻ���Ŀ
    AcademicPaper *academicPapers;// ѧ������
    Competition *competitions;// �������ѧ�ƾ���
    float gpa;  // ѧҵGPA
    float totalCredit;// ��ѧ��
    float qualityGpa;// ���ʼӷ�GPA
    float averageScore;// ��Ȩƽ����
    float totalGPA;// �ܼ���
    struct StudentInfo *next;// ָ����һ��ѧ����ָ��
} StudentInfo;

// ����Ա��Ϣ�ṹ��
typedef struct AdminInfo {
    char username[15];
    char password[15];
    struct AdminInfo *next;
} AdminInfo;

// ȫ�ֱ���
StudentInfo *studentsList = NULL;
AdminInfo *adminsList=NULL;

// ��������
void loginSystem();//��¼ϵͳ
void studentLogin();//ѧ����¼
void adminLogin();//����Ա��¼
void addStudentAccount();//���ѧ���˺�
void addCourse(StudentInfo *student);//��ӿγ�
void addQualityProject(StudentInfo *student);//������ʼӷ���Ŀ
void modifyScoreMenu();//�޸ĳɼ���ɾ��ѧ��ҳ��
void modifyAcademicScore(StudentInfo *student);//�޸�ѧҵ�ɼ�
void modifyQualityScore(StudentInfo *student);//�޸����ʼӷ�
void sortStudentsMenu();//����ѧ��ҳ��
void saveToFile();//���浽�ļ�
void loadFromFile();//���ļ�����
void displayStudentInfo(StudentInfo *student);//��ʾѧ����Ϣ�ſ�
void displayAcademicScore(StudentInfo *student);//��ʾ��ѧ��ȫ��ѧҵ�ɼ�
void displayQualityScore(StudentInfo *student);//��ʾ��ѧ��ȫ�����ʼӷ�
void displayAdminMenu(AdminInfo *admin);//��ʾ����Ա�˵�
void displayStudentMenu(StudentInfo *student);//��ʾѧ���˵�
void freeMemory(StudentInfo **head);//�ͷ��ڴ�
void freeAdminMemory(AdminInfo **head);//�ͷŹ���Ա�ڴ�
StudentInfo *createStudent();//������ѧ���ڵ�
StudentInfo *findStudentByID(const char *studentID);//����ѧ��
AdminInfo *findAdminByUsername(const char *username);//���ҹ���Ա
AcademicScoreNode *createAcademicScore();//������ѧҵ�ɼ��ڵ�
InnovationProject *createInnovationProject();//�����´�ѧ�����´�ҵ�ƻ���Ŀ�ڵ�
AcademicPaper *createAcademicPaper();//������ѧ�����Ľڵ�
Competition *createCompetition();//�����¼������ѧ�ƾ����ڵ�
void insertAcademicScore(StudentInfo *student, AcademicScoreNode *scoreNode);//����ѧҵ�ɼ�����
void insertInnovationProject(StudentInfo *student, InnovationProject *project);//�����ѧ�����´�ҵ�ƻ���Ŀ����β
void insertAcademicPaper(StudentInfo *student, AcademicPaper *paper);//����ѧ����������β
void insertCompetition(StudentInfo *student, Competition *competition);//����������ѧ�ƾ�������β
void insertStudent(StudentInfo **head, StudentInfo *newStudent);//����ѧ������ͷ
void calculateGPA(StudentInfo *student);//����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
float calculateSingleGPA(const float *score);//���㵥�ſγ̼���
void deleteStudentRecord(StudentInfo **head, char *studentID);//ɾ��ѧ����¼
void handleInputError(const char *message);//�����������
void displayAllStudent(StudentInfo *head);//��ʾ����ѧ����Ϣ�ſ�
void sortList(StudentInfo **head, int (*compare)(const StudentInfo *, const StudentInfo *));//����ѧ������
int compareByStudentID(const StudentInfo *a, const StudentInfo *b);//��ѧ������
int compareByGPA(const StudentInfo *a, const StudentInfo *b);//��ѧҵGPA����
int compareByAverageScore(const StudentInfo *a, const StudentInfo *b);//����Ȩƽ��������
int compareByTotalGPA(const StudentInfo *a, const StudentInfo *b);//���ܼ�������
void addAdminAccount();//��ӹ���Ա�˺�
float calculatePaperGPA(AcademicPaper *paper);//����ѧ�����ļӷ�
float calculateCompetitionGPA(Competition *competition);//����������ѧ�ƾ����ӷ�
float calculateInnovationProjectGPA(InnovationProject *project);//�����ѧ�����´�ҵ�ƻ���Ŀ�ӷ�
// ������
int main() {
    // ��ʼ������Ա��Ϣ,�û����������Ϊadmin
    AdminInfo *admin = (AdminInfo *)malloc(sizeof(AdminInfo));
    if (admin == NULL) {
        // ����ڴ����ʧ�ܣ���ӡ������Ϣ���˳�����
        fprintf(stderr, "�ڴ����ʧ��\n");
        exit(EXIT_FAILURE);
    }
    strcpy(admin->username, "admin");
    strcpy(admin->password, "admin");
    admin->next = NULL;
    adminsList = admin;
    // ����ϵͳ
    loginSystem();
    return 0;
}

// ��¼ϵͳ
void loginSystem() {
    int choice;
    printf("��ѡ���¼���ͣ�\n1. ѧ��\n2. ����Ա\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:{
            studentLogin();
            break;
        }
        case 2:{
            adminLogin();
            break;
        }
        default: {
            printf("��Ч��ѡ��\n");
            exit(1);
        }
    }
}

// ѧ����¼
void studentLogin() {
    char studentID[15], password[15];
    printf("������ѧ�ź����룺\n");
    scanf("%s %s", studentID, password);
    // ��֤ѧ����Ϣ
    StudentInfo *student = findStudentByID(studentID);
    if (student && strcmp(student->password, password) == 0) {
        // ��¼�ɹ�
        printf("ѧ����¼�ɹ���\n");
        // ��ʾѧ���˵�
        displayStudentMenu(student);
    } else {
        // ��¼ʧ��
        printf("ѧ�Ż��������\n");
    }
}

// ����Ա��¼
void adminLogin() {
    char username[15];
    printf("���������Ա�û�����\n");
    scanf("%s", username);
    char password[15];
    printf("���������Ա���룺\n");
    scanf("%s", password);
    AdminInfo *adminByUsername = findAdminByUsername(username);
    // ��֤����Ա����
    if (adminByUsername && strcmp(adminByUsername->password, password) == 0) {
        // ��¼�ɹ�
        printf("����Ա��¼�ɹ���\n");
        // ��ʾ����Ա�˵�
        displayAdminMenu(adminByUsername);
    } else {
        // ��¼ʧ��
        printf("�������\n");
        exit(1);
    }
}

// ���ѧ���˺�
void addStudentAccount() {
    StudentInfo *newStudent = createStudent();
    // ����GPA
    calculateGPA(newStudent);
    // ����ѧ���б�
    insertStudent(&studentsList, newStudent);
    printf("ѧ���˺Ŵ����ɹ���\n");
}

// ��ӿγ�
void addCourse(StudentInfo *student) {
    AcademicScoreNode *newCourse = createAcademicScore();
    printf("������γ���Ϣ��\n");
    printf("�γ�����: ");
    scanf("%s", newCourse->courseName);
    printf("����: ");
    scanf("%f", &newCourse->score);
    printf("ѧ��: ");
    scanf("%f", &newCourse->credit);
    newCourse->gpa = calculateSingleGPA(&newCourse->score);
    // ���¿γ���ӵ�ָ��ѧ���Ŀγ�����
    insertAcademicScore(findStudentByID(student->studentID), newCourse);
    printf("�γ���ӳɹ���\n");
}

// ������ʼӷ���Ŀ
void addQualityProject(StudentInfo *student) {
    int choice;
    printf("��ѡ��ӷ���Ŀ���ͣ�\n");
    printf("1. ��ѧ�����´�ҵ�ƻ���Ŀ\n");
    printf("2. ѧ������\n");
    printf("3. �������ѧ�ƾ���\n");
    printf("4. ����\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:{
            InnovationProject *newProject = createInnovationProject();
            insertInnovationProject(student, newProject);
            break;
        }
        case 2:{
            AcademicPaper *newPaper = createAcademicPaper();
            insertAcademicPaper(student, newPaper);
            break;
        }
        case 3:{
            Competition *newCompetition = createCompetition();
            insertCompetition(student, newCompetition);
            break;
        }
        case 4:{
            printf("���ء�\n");
            break;
        }
        default: {
            printf("��Ч��ѡ�\n");
        }
    }
}

// �༭ѧ���ɼ���ɾ��ѧ��
void modifyScoreMenu() {
    char studentID[8];
    printf("������ѧ��ѧ��: ");
    scanf("%s", studentID);
    StudentInfo *student = findStudentByID(studentID);
    if (student != NULL) {
        int choice;
        printf("��ѡ���޸����ͣ�\n");
        printf("1. �޸�ѧҵ�ɼ�\n");
        printf("2. ���ѧҵ�ɼ�\n");
        printf("3. �޸����ʼӷ�\n");
        printf("4. ������ʼӷ�\n");
        printf("5. ɾ��ѧ��\n");
        printf("6. ����\n");
        scanf("%d", &choice);
        switch (choice) {
            case 1:{
                // �༭ѧҵ�ɼ�
                modifyAcademicScore(student);
                break;
            }
            case 2:{
                // ���ѧҵ�ɼ�
                addCourse(student);
                break;
            }
            case 3:{
                // �༭���ʼӷ�
                modifyQualityScore(student);
                break;
            }
            case 4:{
                // ������ʼӷ�
                addQualityProject(student);
                break;
            }
            case 5:{
                // ɾ��ѧ��
                deleteStudentRecord(&studentsList, studentID);
                break;
            }
            case 6:{
                printf("���ء�\n");
                // ������һ���˵�
                break;
            }
            default: {
                printf("��Ч��ѡ�\n");
            }
        }
        // ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
        calculateGPA(student);
        printf("�ɼ��޸ĳɹ���\n");
    } else {
        printf("δ�ҵ�ѧ����Ϣ��\n");
    }
}

// ����ѧ��
void sortStudentsMenu() {
    // ����GPA��������׼��ѧ���б��������
    int choice;
    printf("��ѡ������ʽ��\n");
    printf("1. ��ѧ������\n");
    printf("2. ��ѧҵGPA����\n");
    printf("3. ����Ȩƽ��������\n");
    printf("4. �����ܼ�������\n");
    printf("5. ����\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:{
            // ��ѧ������
            sortList(&studentsList, compareByStudentID);
            break;
        }
        case 2:{
            // ��ѧҵGPA����
            sortList(&studentsList, compareByGPA);
            break;
        }
        case 3:{
            // ����Ȩƽ��������
            sortList(&studentsList, compareByAverageScore);
            break;
        }
        case 4:{
            // �����ܼ�������
            sortList(&studentsList, compareByTotalGPA);
            break;
        }
        case 5:{
            printf("���ء�\n");
            // ������һ���˵�
            break;
        }
        default:{
            printf("��Ч��ѡ�\n");
        }
    }
    printf("ѧ��������ɡ�\n");
}

// ��ʾѧ����Ϣ�ſ�
void displayStudentInfo(StudentInfo *student) {
    // ��ʾѧ���ĳɼ��ͼӷ���Ϣ
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    printf("ѧ��: %s\n", student->studentID);
    printf("����: %s\n", student->name);
    printf("�༶: %d\n", student->classNumber);
    printf("�ѻ���ѧ��: %.2f\n", student->totalCredit);
    printf("ѧҵGPA: %f\n", student->gpa);
    printf("���ʼӷ�GPA: %.2f\n", student->qualityGpa);
    printf("��Ȩƽ����: %.2f\n", student->averageScore);
    printf("�ܼ���: %.2f\n", student->totalGPA);
    printf("ѧ����Ϣ��ʾ��ɡ�\n");
    int choice;
    printf("�Ƿ�Ҫ������ʾ��ϸ��Ϣ��\n");
    printf("1. ��ʾ����ѧҵ�ɼ�\n");
    printf("2. ��ʾ�������ʼӷ�\n");
    printf("3. ����\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:{
            displayAcademicScore(student);
            break;
        }
        case 2:{
            displayQualityScore(student);
            break;
        }
        case 3:{
            printf("���ء�\n");
            break;
        }
        default: {
            printf("��Ч��ѡ�\n");
        }
    }
}

// ��ʾ����Ա�˵�
void displayAdminMenu(AdminInfo *admin) {
    int choice;
    do {
        printf("����Ա�˵���\n");
        printf("1. ���ѧ���˺�\n");
        printf("2. ��ӹ���Ա�˺�\n");
        printf("3. �༭ѧ���ɼ���ɾ��ѧ��\n");
        printf("4. ����ѧ��\n");
        printf("5. ��ʾѧ����Ϣ\n");
        printf("6. ����ѧ��\n");
        printf("7. ���浽�ļ�\n");
        printf("8. ���ļ�����\n");
        printf("9. ��������\n");
        printf("10. �˳�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice) {
            case 1:{
                // ���ѧ���˺�
                addStudentAccount();
                break;
            }
            case 2:{
                // ��ӹ���Ա�˺�
                addAdminAccount();
                break;
            }
            case 3:{
                // �༭ѧ���ɼ���ɾ��ѧ��
                modifyScoreMenu();
                break;
            }
            case 4:{
                // ����ѧ��
                sortStudentsMenu();
                break;
            }
            case 5:{
                // ��ʾѧ����Ϣ
                displayAllStudent(studentsList);
                break;
            }
            case 6:{
                // ����
                printf("��������Ҫ����ѧ����ѧ��\n");
                char studentID[8];
                scanf("%s", studentID);
                StudentInfo *student = findStudentByID(studentID);
                if (student != NULL) {
                    displayStudentInfo(student);
                } else {
                    printf("δ�ҵ�ѧ����Ϣ��\n");
                }
                break;
            }
            case 7:{
                // ���浽�ļ�
                saveToFile();
                break;
            }
            case 8:{
                // ���ļ�����
                loadFromFile();
                break;
            }
            case 9:{
                // ��������
                char newPassword[15];
                printf("�����������룺");
                scanf("%s", newPassword);
                strcpy(admin->password, newPassword);
                printf("�����޸ĳɹ���\n");
                break;
            }
            case 10:{
                printf("����Ա�˳���¼��\n");
                freeMemory(&studentsList);//�ͷ��ڴ�
                freeAdminMemory(&adminsList);//�ͷ��ڴ�
                break;
            }
            default: {
                printf("��Ч��ѡ����������롣\n");
            }
        }
    } while (choice != 10);
}

//��ʾѧ���˵�
void displayStudentMenu(StudentInfo *student) {
    int choice;
    do {
        printf("ѧ���˵���\n");
        printf("1. �鿴�ɼ�\n");
        printf("2. �鿴�ӷ�\n");
        printf("3. �޸�����\n");
        printf("4. �˳�\n");
        printf("��ѡ�������");
        scanf("%d", &choice);
        switch (choice) {
            case 1:{
                // �鿴�ɼ�
                displayAcademicScore(student);
                break;
            }
            case 2:{
                // �鿴�ӷ�
                displayQualityScore(student);
                break;
            }
            case 3:{
                // �޸�����
                char newPassword[15];
                printf("�����������룺");
                scanf("%s", newPassword);
                strcpy(student->password, newPassword);
                printf("�����޸ĳɹ���\n");
                break;
            }
            case 4:{
                printf("ѧ���˳���¼��\n");
                freeMemory(&studentsList);//�ͷ��ڴ�
                freeAdminMemory(&adminsList);//�ͷ��ڴ�
                break;
            }
            default: {
                printf("��Ч��ѡ����������롣\n");
            }
        }
    } while (choice != 4);
}
// �ͷ�ѧ���ڴ�
void freeMemory(StudentInfo **head) {
    // ���ѧ���б�ͷָ���Ƿ�Ϊ��
    if (*head == NULL) {
        return; // ���ͷָ��Ϊ�գ���û���ڴ���Ҫ�ͷ�
    }

    // �������ͷ�ÿ��ѧ���ڵ��ѧҵ�ɼ�����
    StudentInfo *currentStudent = *head;
    while (currentStudent != NULL) {
        // �ͷ�ѧҵ�ɼ������е�ÿ���ڵ�
        AcademicScoreNode *currentScore = currentStudent->academicScores;
        while (currentScore != NULL) {
            AcademicScoreNode *temp = currentScore;
            currentScore = currentScore->next;
            free(temp); // �ͷŽڵ��ڴ�
        }
        // �ͷŴ�ѧ�����´�ҵ�ƻ���Ŀ�����е�ÿ���ڵ�
        InnovationProject *currentProject = currentStudent->innovationProjects;
        if (currentProject != NULL) {
            InnovationProject *temp = currentProject;
            free(temp); // �ͷŽڵ��ڴ�
        }
        // �ͷ�ѧ�����������е�ÿ���ڵ�
        AcademicPaper *currentPaper = currentStudent->academicPapers;
        while (currentPaper != NULL) {
            AcademicPaper *temp = currentPaper;
            currentPaper = currentPaper->next;
            free(temp); // �ͷŽڵ��ڴ�
        }
        // �ͷż������ѧ�ƾ��������е�ÿ���ڵ�
        Competition *currentCompetition = currentStudent->competitions;
        while (currentCompetition != NULL) {
            Competition *temp = currentCompetition;
            currentCompetition = currentCompetition->next;
            free(temp); // �ͷŽڵ��ڴ�
        }


        // �ͷŵ�ǰѧ���ڵ���ڴ�
        StudentInfo *tempStudent = currentStudent;
        currentStudent = currentStudent->next;
        free(tempStudent); // �ͷŽڵ��ڴ�
    }

    // ����ѧ���б�ͷָ��ΪNULL
    *head = NULL;
}

// ����ѧ����¼
void insertStudent(StudentInfo **head, StudentInfo *newStudent) {
    // ����ѧ�����뵽ѧ���б��ͷ��
    if (head == NULL || newStudent == NULL) {
        return; // ���ͷָ�����ѧ���ڵ�Ϊ�գ���ִ�в���
    }

    // ����ѧ���ڵ��nextָ��ָ��ǰ��ͷ�ڵ�
    newStudent->next = *head;

    // ����ͷָ��Ϊ��ѧ���ڵ�
    *head = newStudent;
}

// ����ѧҵ�ɼ�
void insertAcademicScore(StudentInfo *student, AcademicScoreNode *scoreNode) {
    // ���¿γ���ӵ�ѧ����ѧҵ�ɼ�����
    if (student == NULL || scoreNode == NULL) {
        return; // ���ѧ����ɼ��ڵ�Ϊ�գ���ִ�в���
    }

    AcademicScoreNode *current = student->academicScores;
    if (current == NULL) {
        // ���ѧ���ĳɼ�����Ϊ�գ����½ڵ��Ϊ��һ���ڵ�
        student->academicScores = scoreNode;
    } else {
        // ��������ֱ���ҵ����һ���ڵ�
        while (current->next != NULL) {
            current = current->next;
        }
        // ���½ڵ���ӵ������ĩβ
        current->next = scoreNode;
    }
}

// �����ѧ�����´�ҵ�ƻ���Ŀ
void insertInnovationProject(StudentInfo *student, InnovationProject *project) {
    // ������Ŀ��ӵ�ѧ���Ĵ�ѧ�����´�ҵ�ƻ���Ŀ����
    if (student == NULL ) {
        return; // ���ѧ������Ŀ�ڵ�Ϊ�գ���ִ�в���
    }

    InnovationProject *current = student->innovationProjects;
    if (current == NULL) {
        // ���ѧ������Ŀ����Ϊ�գ����½ڵ��Ϊ��һ���ڵ�
        student->innovationProjects = project;
    } else {
        printf("�Ѿ����ڴ�ѧ�����´�ҵ�ƻ���Ŀ");
    }
}

// ����ѧ������
void insertAcademicPaper(StudentInfo *student, AcademicPaper *paper) {
    // ����������ӵ�ѧ����ѧ����������
    if (student == NULL || paper == NULL) {
        return; // ���ѧ�������Ľڵ�Ϊ�գ���ִ�в���
    }

    AcademicPaper *current = student->academicPapers;
    if (current == NULL) {
        // ���ѧ������������Ϊ�գ����½ڵ��Ϊ��һ���ڵ�
        student->academicPapers = paper;
    } else {
        // ��������ֱ���ҵ����һ���ڵ�
        while (current->next != NULL) {
            current = current->next;
        }
        // ���½ڵ���ӵ������ĩβ
        current->next = paper;
    }
}

// ����������ѧ�ƾ���
void insertCompetition(StudentInfo *student, Competition *competition) {
    // ���¾�����ӵ�ѧ���ļ������ѧ�ƾ�������
    if (student == NULL || competition == NULL) {
        return; // ���ѧ�������ڵ�Ϊ�գ���ִ�в���
    }

    Competition *current = student->competitions;
    if (current == NULL) {
        // ���ѧ���ľ�������Ϊ�գ����½ڵ��Ϊ��һ���ڵ�
        student->competitions = competition;
    } else {
        // ��������ֱ���ҵ����һ���ڵ�
        while (current->next != NULL) {
            current = current->next;
        }
        // ���½ڵ���ӵ������ĩβ
        current->next = competition;
    }
}

// ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
void calculateGPA(StudentInfo *student) {
    // ���㲢����ѧ����GPA
    if (student == NULL) {
        return; // ��������ѧ��ָ��Ϊ�գ���ֱ�ӷ���
    }
    float totalQualityPoints = 0.00f; // �����ۼӿγ̵�GPAֵ
    int numCourses = 0; // ���ڼ����γ�����
    float totalCredit = 0.00f;// �����ۼ�ѧ��
    float totalScore = 0.00f; // ���ڼ����Ȩƽ����


    // ����ѧ����ѧҵ�ɼ�����
    AcademicScoreNode *currentScore = student->academicScores;
    while (currentScore != NULL) {
        totalQualityPoints += currentScore->gpa * currentScore->credit; // �ۼ�ÿ���γ̵�GPAֵ��ѧ��
        numCourses++;     // �γ���������
        totalCredit += currentScore->credit;    // �ۼ�ѧ��
        totalScore += currentScore->score * currentScore->credit; // �ۼӷ���
        currentScore = currentScore->next; // �ƶ�����һ���γ̽ڵ�
    }

    // ��ѧ��
    student->totalCredit = totalCredit;

    // �����ȨѧҵGPA
    if (numCourses > 0) {
        student->gpa = totalQualityPoints / totalCredit; // ����ѧҵGPA
    } else {
        student->gpa = 0.00f; // ���û�пγ̣�GPAΪ0
    }

    // �ܼ���
    student->totalGPA = student->gpa + student->qualityGpa;

    // �����Ȩƽ����
    student->averageScore = totalScore / totalCredit;
}

// ���㵥�ſγ̼���
float calculateSingleGPA(const float *score) {
    if (*score >= 90) {
        return 4.0f;
    } else if (*score >= 87) {
        return 3.7f;
    } else if (*score >= 83) {
        return 3.3f;
    } else if (*score >= 80) {
        return 3.0f;
    } else if (*score >= 77) {
        return 2.7f;
    } else if (*score >= 73) {
        return 2.3f;
    } else if (*score >= 70) {
        return 2.0f;
    } else if (*score >= 67) {
        return 1.7f;
    } else if (*score >= 63) {
        return 1.3f;
    }else if (*score >= 60) {
        return 1.0f;
    } else {
        return 0.0f;
    }
}

// ɾ��ѧ����¼
void deleteStudentRecord(StudentInfo **head, char *studentID) {
    // ����ѧ��ɾ��ѧ����¼
    // ʹ��findStudentByID��������ѧ��
    StudentInfo *studentToDelete = findStudentByID(studentID);
    if (studentToDelete != NULL) {
        // ����ҵ���ѧ������������ɾ��
        if (*head == studentToDelete) {
            // ���Ҫɾ������ͷ�ڵ�
            *head = (*head)->next;
        } else {
            // ���Ҫɾ�����������м��β���Ľڵ�
            StudentInfo *current = *head;
            while (current->next != studentToDelete) {
                current = current->next;
            }
            current->next = current->next->next;
        }
        free(studentToDelete); // �ͷ�ѧ����¼���ڴ�
        printf("ѧ����¼ɾ���ɹ���\n");
    } else {
        // ���û���ҵ�ѧ��IDƥ��Ľڵ㣬��ӡ������Ϣ
        printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����¼��\n", studentID);
    }
}

// �����������
void handleInputError(const char *message) {
    // ��ʾ������Ϣ
    printf("%s\n", message);
}

// ������ѧ��
StudentInfo *createStudent() {
    // �����ڴ���µ�StudentInfoʵ��
    StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
    if (newStudent == NULL) {
        // ����ڴ����ʧ�ܣ���ӡ������Ϣ���˳�����
        fprintf(stderr, "�ڴ����ʧ��\n");
        exit(EXIT_FAILURE);
    }

    // ��ʼ����ѧ����Ϣ
    newStudent->next = NULL; // ��ʼ������ָ��
    newStudent->academicScores = NULL; // ��ʼ��ѧҵ�ɼ�����
    newStudent->innovationProjects = NULL; // ��ʼ����ѧ�����´�ҵ�ƻ���Ŀ����
    newStudent->academicPapers = NULL; // ��ʼ��ѧ����������
    newStudent->competitions = NULL; // ��ʼ���������ѧ�ƾ�������
    newStudent->gpa = 0.00f; // ��ʼ��GPA
    newStudent->totalCredit = 0; // ��ʼ����ѧ��
    newStudent->qualityGpa = 0.00f; // ��ʼ�����ʼӷ�GPA
    newStudent->averageScore = 0.00f; // ��ʼ����Ȩƽ����
    strcpy(newStudent->name, "New Student");
    strcpy(newStudent->studentID, "00000000");
    newStudent->classNumber = 1; // ����ѧ���༶Ϊ1
    strcpy(newStudent->password, "123456"); // Ĭ������Ϊ123456

    // ����ѧ����Ϣ
    printf("������ѧ����Ϣ��\n");
    printf("ѧ��: ");
    scanf("%s", newStudent->studentID);
    // ���ѧ���Ƿ��Ѵ���
    if (findStudentByID(newStudent->studentID) != NULL) {
        // ���ѧ���Ѵ��ڣ���ӡ������Ϣ���ͷ��ڴ沢������һ���˵�
        handleInputError("ѧ���Ѵ��ڣ����������롣");
        free(newStudent);
        return NULL;
    }
    printf("����: ");
    scanf("%s", newStudent->name);
    printf("�༶: ");
    scanf("%d", &newStudent->classNumber);
//    printf("����: ");
//    scanf("%s", newStudent->password);
    //¼��ѧҵ�ɼ�
    int choice1;
    printf("�Ƿ�����¼��γ̼�¼��(1. �� 2. ��): ");
    scanf("%d", &choice1);
    if(choice1 == 1){
        int numCourses;
        printf("������ѧ���Ŀγ�������");
        scanf("%d", &numCourses);
        for (int i = 0; i < numCourses; i++) {
            AcademicScoreNode *newCourse = createAcademicScore();
            printf("�������%d�ſγ���Ϣ��\n", i + 1);
            addCourse(newStudent);
        }
    }

    int choice2;
    printf("�Ƿ�����¼�����ʼӷ���Ŀ��¼��(1. �� 2. ��): ");
    scanf("%d", &choice2);
    if(choice2 == 1){
        printf("¼��");
    }

    // �����´�����StudentInfoָ��
    return newStudent;
}

// ������ѧҵ�ɼ�
AcademicScoreNode *createAcademicScore() {
        // �����ڴ���µ�AcademicScoreNodeʵ��
        AcademicScoreNode *newCourse = (AcademicScoreNode *)malloc(sizeof(AcademicScoreNode));
        if (newCourse == NULL) {
            // ����ڴ����ʧ�ܣ���ӡ������Ϣ���˳�����
            fprintf(stderr, "�ڴ����ʧ��\n");
            exit(EXIT_FAILURE);
        }
        // ��ʼ���¿γ���Ϣ
        newCourse->next = NULL; // ��ʼ������ָ��
        strcpy(newCourse->courseName, "Unknown Course"); // ��ʼ���γ�����
        newCourse->score = 0.00f; // ��ʼ���γ̷���
        newCourse->credit = 0.0f; // ��ʼ���γ�ѧ��
        newCourse->gpa = 0.0f; // ��ʼ���γ̼���
        printf("������γ���Ϣ��\n");
        printf("�γ�����: ");
        scanf("%s", newCourse->courseName);
        printf("����: ");
        scanf("%f", &newCourse->score);
        printf("ѧ��: ");
        scanf("%f", &newCourse->credit);
        newCourse->gpa = calculateSingleGPA(&newCourse->score); // ����γ̼���
        // �����´�����AcademicScoreNodeָ��
        return newCourse;
}


// �����´�ѧ�����´�ҵ�ƻ���Ŀ
InnovationProject *createInnovationProject() {
    InnovationProject *newProject = (InnovationProject *)malloc(sizeof(InnovationProject));
    if (newProject == NULL) {
        // ����ڴ����ʧ�ܣ���ӡ������Ϣ���˳�����
        fprintf(stderr, "�ڴ����ʧ��\n");
        exit(EXIT_FAILURE);
    }
    newProject->gpa = 0.00f; // Ĭ�ϼӷ�ֵ��ʼ��Ϊ0.0
    printf("�������ѧ�����´�ҵ�ƻ���Ŀ��Ϣ��\n");
    printf("��Ŀ����: ");
    scanf("%s", newProject->projectName);
    newProject->gpa = calculateInnovationProjectGPA(newProject);
    printf("�Ƿ�Ϊ��Ŀ������(1. �� 2. ��): ");
    scanf("%d", &newProject->isLeaderOrSecondLeader);
    printf("��Ŀ����(���Ҽ���ʡ����У��): ");
    scanf("%s", newProject->projectLevel);
    printf("�Ƿ����(1. �� 2. ��): ");
    scanf("%d", &newProject->isFinished);

    // �����´�����InnovationProjectָ��
    return newProject;
}

// ������ѧ������
AcademicPaper *createAcademicPaper() {
    AcademicPaper *newPaper = (AcademicPaper *)malloc(sizeof(AcademicPaper));
    if (newPaper == NULL) {
        // ����ڴ����ʧ�ܣ���ӡ������Ϣ���˳�����
        fprintf(stderr, "�ڴ����ʧ��\n");
        exit(EXIT_FAILURE);
    }
    newPaper->gpa = 0.00f; // Ĭ�ϼӷ�ֵ��ʼ��Ϊ0.0
    printf("������ѧ��������Ϣ��\n");
    printf("���ı���: ");
    scanf("%s", newPaper->title);
    printf("�ڿ�����: ");
    scanf("%s", newPaper->journalName);
    printf("�Ƿ�Ϊ��һ����(1. �� 2. ��): ");
    scanf("%d", &newPaper->isFirstAuthor);
    printf("�ڿ�����(1. ���� 2. һ�� 3. ���� 4. ���� 5. �ļ� 6. �弶): ");
    scanf("%d", &newPaper->journalLevel);
    newPaper->gpa = calculatePaperGPA(newPaper);
    // �����´�����AcademicPaperָ��
    return newPaper;
}

// �����¼������ѧ�ƾ���
Competition *createCompetition() {
    Competition *newCompetition = (Competition *)malloc(sizeof(Competition));
    if (newCompetition == NULL) {
        // ����ڴ����ʧ�ܣ���ӡ������Ϣ���˳�����
        fprintf(stderr, "�ڴ����ʧ��\n");
        exit(EXIT_FAILURE);
    }
    newCompetition->gpa = 0.00f; // Ĭ�ϼӷ�ֵ��ʼ��Ϊ0.0
    printf("������������ѧ�ƾ�����Ϣ��\n");
    printf("��������: ");
    scanf("%s", newCompetition->competitionName);
    printf("������������: ");
    scanf("%d", &newCompetition->teamSize);
    printf("��������(���Ҽ���ʡ����У��): ");
    scanf("%s", newCompetition->competitionLevel);
    printf("�񽱵ȼ�(1. һ�Ƚ� 2. ���Ƚ� 3. ���Ƚ�): ");
    scanf("%d", &newCompetition->rank);
    printf("�������(A�࣬B�࣬C��): ");
    scanf("%s", newCompetition->competitionType);
    newCompetition->gpa = calculateCompetitionGPA(newCompetition);
    // �����´�����Competitionָ��
    return newCompetition;
}
// ����ѧ��
StudentInfo *findStudentByID(const char *studentID) {
    // ����ѧ���б�����ָ��ѧ�ŵ�ѧ��
    // ��ѧ���б��ͷ����ʼ����
    StudentInfo *currentStudent = studentsList;

    // ��������ѧ���б�
    while (currentStudent != NULL) {
        // �Ƚϵ�ǰѧ����ѧ���������ѧ��
        if (strcmp(currentStudent->studentID, studentID) == 0) {
            // ����ҵ�ƥ���ѧ�ţ����ص�ǰѧ������Ϣ
            return currentStudent;
        }
        // �ƶ����������һ���ڵ�
        currentStudent = currentStudent->next;
    }
    // ��������������б�û���ҵ�ƥ���ѧ�ţ�����NULL
    return NULL;
}

// ���ҹ���Ա
AdminInfo *findAdminByUsername(const char *username) {
    // �Ƚϸ������û����͹���Ա���û���
    if (strcmp(adminsList->username, username) == 0) {
        // ����ҵ�ƥ����û��������ع���Ա��Ϣ
        return adminsList;
    }
    // ���û���ҵ�ƥ����û���������NULL
    return NULL;
}

// �༭ѧ��ѧҵ�ɼ�
void modifyAcademicScore(StudentInfo *student) {
    // �޸�ѧҵ�ɼ�
    int choice;
    printf("��ѡ���޸����ͣ�\n");
    printf("1. �޸Ŀγ�����\n");
    printf("2. �޸ķ���\n");
    printf("3. �޸�ѧ��\n");
    printf("4. ɾ���γ�\n");
    printf("5. ������һ���˵�\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:{
            // �޸Ŀγ�����
            if (student->academicScores == NULL) {
                printf("��ǰû�пγ̿����޸ġ�\n");
                break;
            }
            AcademicScoreNode *current = student->academicScores;
            printf("������Ҫ�޸ĵĿγ����ƣ�����q�˳�����");
            char courseName[MAX_NAME_LENGTH];
            scanf("%s", courseName);
            if (strcmp(courseName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->courseName, courseName) == 0) {
                    printf("�ҵ��γ�: %s, ��ǰ����: %.2f\n", current->courseName, current->score);
                    printf("�������µĿγ�����: ");
                    scanf("%s", current->courseName);
                    printf("�γ������޸ĳɹ���\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("δ�ҵ�ָ���Ŀγ̡�\n");
            }
            break;
        }
        case 2:{
            // �޸ķ���
            if (student->academicScores == NULL) {
                printf("��ǰû�пγ̿����޸ġ�\n");
                break;
            }
            AcademicScoreNode *currentScore = student->academicScores;
            printf("������Ҫ�޸ĵĿγ����ƣ�����q�˳�����");
            char scoreCourseName[MAX_NAME_LENGTH];
            scanf("%s", scoreCourseName);
            if (strcmp(scoreCourseName, "q") == 0) {
                break;
            }
            while (currentScore != NULL) {
                if (strcmp(currentScore->courseName, scoreCourseName) == 0) {
                    printf("�ҵ��γ�: %s, ��ǰ����: %.2f\n", currentScore->courseName, currentScore->score);
                    printf("�������µķ���: ");
                    scanf("%2f", &currentScore->score);
                    currentScore->gpa = calculateSingleGPA(&currentScore->score);//���¼��㵱ǰ�γ̼���
                    calculateGPA(student);//���¼���GPA\��ѧ��\��Ȩƽ����\�ܼ���
                    printf("�����޸ĳɹ���\n");
                    break;
                }
                currentScore = currentScore->next;
            }
            if (currentScore == NULL) {
                printf("δ�ҵ�ָ���Ŀγ̡�\n");
            }
            break;
        }
        case 3:{
            // �޸�ѧ��
            if (student->academicScores == NULL) {
                printf("��ǰû�пγ̿����޸ġ�\n");
                break;
            }
            AcademicScoreNode *currentCredit = student->academicScores;
            printf("������Ҫ�޸ĵĿγ����ƣ�����q�˳�����");
            char creditCourseName[MAX_NAME_LENGTH];
            scanf("%s", creditCourseName);
            if (strcmp(creditCourseName, "q") == 0) {
                break;
            }
            while (currentCredit != NULL) {
                if (strcmp(currentCredit->courseName, creditCourseName) == 0) {
                    printf("�ҵ��γ�: %s, ��ǰѧ��: %.2f\n", currentCredit->courseName, currentCredit->credit);
                    printf("�������µ�ѧ��: ");
                    scanf("%2f", &currentCredit->credit);
                    calculateGPA(student);  //���¼���GPA\��ѧ��\��Ȩƽ����\�ܼ���
                    printf("ѧ���޸ĳɹ���\n");
                    break;
                }
                currentCredit = currentCredit->next;
            }
            if (currentCredit == NULL) {
                printf("δ�ҵ�ָ���Ŀγ̡�\n");
            }
            break;
        }
        case 4:{
            // ɾ���γ�
            if (student->academicScores == NULL) {
                printf("��ǰû�пγ̿���ɾ����\n");
                break;
            }
            AcademicScoreNode *currentDelete = student->academicScores;
            AcademicScoreNode *prev = NULL;
            printf("������Ҫɾ���Ŀγ����ƣ�����q�˳�����");
            char deleteCourseName[MAX_NAME_LENGTH];
            scanf("%s", deleteCourseName);
            if (strcmp(deleteCourseName, "q") == 0) {
                break;
            }
            while (currentDelete != NULL) {
                if (strcmp(currentDelete->courseName, deleteCourseName) == 0) {
                    if (prev == NULL) {
                        // Ҫɾ������ͷ�ڵ�
                        student->academicScores = currentDelete->next;
                    } else {
                        // Ҫɾ�������м��β�ڵ�
                        prev->next = currentDelete->next;
                    }
                    AcademicScoreNode *temp = currentDelete;
                    currentDelete = currentDelete->next;
                    free(temp); // �ͷű�ɾ���ڵ���ڴ�
                    calculateGPA(student);  //���¼���GPA\��ѧ��\��Ȩƽ����\�ܼ���
                    printf("�γ�ɾ���ɹ���\n");
                    break;
                }
                prev = currentDelete;
                currentDelete = currentDelete->next;
            }
            if (currentDelete == NULL) {
                printf("δ�ҵ�ָ���Ŀγ̡�\n");
            }
            break;
        }
        case 5:{
            printf("������һ���˵���\n");
            // ������һ���˵�
            break;
        }
        default: {
            printf("��Ч��ѡ�\n");
        }
    }
}

// �༭ѧ�����ʼӷ�
void modifyQualityScore(StudentInfo *student) {
    // �޸����ʼӷ�
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }

    int choice;
    printf("��ѡ���޸����ͣ�\n");
    printf("1. �޸Ĵ�ѧ�����´�ҵ��Ŀ\n");
    printf("2. ɾ����ѧ�����´�ҵ��Ŀ\n");
    printf("3. �޸�ѧ������\n");
    printf("4. ɾ��ѧ������\n");
    printf("5. �޸ļ������ѧ�ƾ���\n");
    printf("6. ɾ���������ѧ�ƾ���\n");
    printf("7. ������һ���˵�\n");
    scanf("%d", &choice);
    // �����û�ѡ��ִ����Ӧ����
    switch (choice) {

    }
}

// ��ʾѧ����ȫ��ѧҵ�ɼ�
void displayAcademicScore(StudentInfo *student) {
    // ��ʾѧ����ѧҵ�ɼ�
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    AcademicScoreNode *current = student->academicScores;
    if (current == NULL) {
        printf("ѧ��û��ѧҵ�ɼ���\n");
        return;
    }
    printf("ѧҵ�ɼ���\n");
    while (current != NULL) {
        printf("�γ�����: %s, ѧ��: %.2f, ����: %.2f, ����: %.2f\n", current->courseName, current->credit, current->score, current->gpa);
        current = current->next;
    }
    printf("����ѧҵ�ɼ���ȫ����ʾ��ɡ�\n");
}

// ��ʾѧ����ȫ�����ʼӷ�
void displayQualityScore(StudentInfo *student) {
    // ��ʾѧ�������ʼӷ�
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    // ��ʾ��ѧ�����´�ҵ�ƻ���Ŀ
    InnovationProject *currentProject = student->innovationProjects;
    if (currentProject == NULL) {
        printf("ѧ��û�д�ѧ�����´�ҵ�ƻ���Ŀ��\n");
    } else {
        printf("��ѧ�����´�ҵ�ƻ���Ŀ��\n");
        if (currentProject != NULL) {
            printf("��Ŀ����: %s, �ӷ�: %.2f\n", currentProject->projectName, currentProject->gpa);
        }else{
            printf("�޴�ѧ�����´�ҵ�ƻ���Ŀ��\n");
        }
    }
    printf("�������ʼӷ���ȫ����ʾ��ɡ�\n");
}

//�ͷŹ���Ա�ڴ�
void freeAdminMemory(AdminInfo **head) {
    // ������Ա�б�ͷָ���Ƿ�Ϊ��
    if (*head == NULL) {
        return; // ���ͷָ��Ϊ�գ���û���ڴ���Ҫ�ͷ�
    }
    // �ͷŵ�ǰ����Ա�ڵ���ڴ�
    AdminInfo *tempAdmin = *head;
    *head = NULL;
    free(tempAdmin); // �ͷŽڵ��ڴ�
}

//��ʾ����ѧ���ſ�
void displayAllStudent(StudentInfo *head) {
    // ��ʾ����ѧ������Ϣ
    if (head == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    StudentInfo *current = head;
    while (current != NULL) {
        printf("ѧ��: %s\n", current->studentID);
        printf("����: %s\n", current->name);
        printf("�༶: %d\n", current->classNumber);
        printf("�ѻ���ѧ��: %.2f\n", current->totalCredit);
        printf("ѧҵGPA: %f\n", current->gpa);
        printf("���ʼӷ�GPA: %.2f\n", current->qualityGpa);
        printf("��Ȩƽ����: %.2f\n", current->averageScore);
        printf("�ܼ���: %.2f\n", current->totalGPA);
        printf("\n\n");
        current = current->next;
    }
    printf("\n");
    printf("����ѧ����Ϣ��ȫ����ʾ��ɡ�\n");
}

//����ѧ������
void sortList(StudentInfo **head, int (*compare)(const StudentInfo *, const StudentInfo *)) {
    int swapped;
    StudentInfo *current;
    StudentInfo *prev = NULL;
    do {
        swapped = 0;
        current = *head;
        while (current && current->next) {
            if ((*compare)(current, current->next) > 0) {
                // ���������ڵ������
                StudentInfo temp = *current;
                *current = *(current->next);
                *(current->next) = temp;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}

// �ȽϺ�������ѧ������
int compareByStudentID(const StudentInfo *a, const StudentInfo *b) {
    return strcmp(a->studentID, b->studentID);
}

// �ȽϺ�������ѧҵGPA����
int compareByGPA(const StudentInfo *a, const StudentInfo *b) {
    if (a->gpa > b->gpa) return -1;
    if (a->gpa < b->gpa) return 1;
    return 0;
}

// �ȽϺ���������Ȩƽ��������
int compareByAverageScore(const StudentInfo *a, const StudentInfo *b) {
    if (a->averageScore > b->averageScore) return -1;
    if (a->averageScore < b->averageScore) return 1;
    return 0;
}

// �ȽϺ��������ܼ�������
int compareByTotalGPA(const StudentInfo *a, const StudentInfo *b) {
    if (a->totalGPA > b->totalGPA) return -1;
    if (a->totalGPA < b->totalGPA) return 1;
    return 0;
}

// ��ӹ���Ա�˺�
void addAdminAccount() {
    AdminInfo *newAdmin = (AdminInfo *)malloc(sizeof(AdminInfo)); // �����¹���Ա�ڴ�
    if (newAdmin == NULL) {
        handleInputError("�ڴ����ʧ�ܣ��޷���ӹ���Ա�˺š�\n");
        return;
    }

    printf("���������Ա�û���: ");
    scanf("%s", newAdmin->username);
    // ����û����Ƿ��Ѵ���
    while (findAdminByUsername(newAdmin->username) != NULL) {
        printf("�û����Ѵ��ڣ�����������: ");
        scanf("%s", newAdmin->username);
    }

    printf("���������Ա����: ");
    scanf("%s", newAdmin->password);

    // ���¹���Ա��ӵ�����ͷ��
    newAdmin->next = adminsList;
    adminsList = newAdmin;

    printf("����Ա�˺���ӳɹ���\n");
}

// ���浽�ļ�
void saveToFile() {

}

// ���ļ�����
void loadFromFile() {

}