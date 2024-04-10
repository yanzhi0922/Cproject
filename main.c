#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ��������ַ�������
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 15
#define STUDENT_ID_LENGTH 8
#define MAX_CREDIT 8

// �ض��ַ������ڶ�ȡ���
#define STUDENT_START "##StudentStart##"
#define STUDENT_END "##StudentEnd##"
#define SCORE_START "##ScoreStart##"
#define SCORE_END "##ScoreEnd##"
#define PROJECT_START "##ProjectStart##"
#define PROJECT_END "##ProjectEnd##"
#define PAPER_START "##PaperStart##"
#define PAPER_END "##PaperEnd##"
#define COMPETITION_START "##CompetitionStart##"
#define COMPETITION_END "##CompetitionEnd##"
#define ADMIN_START "##AdminStart##"
#define ADMIN_END "##AdminEnd##"

// ѧҵ�ɼ��ṹ��
typedef struct AcademicScoreNode {
    char courseName[MAX_NAME_LENGTH];// �γ���
    float score;    // ����
    float credit; // ѧ��
    float gpa; // ���ſγ̼���
    struct AcademicScoreNode *next;
} AcademicScoreNode;

// ��ѧ�����´�ҵ�ƻ���Ŀ�ṹ��
typedef struct InnovationProject {
    char projectName[MAX_NAME_LENGTH]; // ��Ŀ����
    float gpa; // �ӷ�ֵ
    int isLeaderOrSecondLeader;// �Ƿ�Ϊ��Ŀ������,�Ƿ�Ϊ�ڶ�������
    char projectLevel[5];// ��Ŀ���𣬹��Ҽ���ʡ����У��
    int isFinished;    // �Ƿ����
} InnovationProject;

// ѧ�����Ľṹ��
typedef struct AcademicPaper {
    char title[MAX_NAME_LENGTH]; // ���ı���
    char journalName[MAX_NAME_LENGTH]; // �ڿ�����
    int isFirstAuthor; // �Ƿ�Ϊ��һ����
    int journalLevel; //�ڿ��������ڿ������Զ��жϣ��˴�ֻ�������ֱ��
    float gpa; // �ӷ�ֵ
    struct AcademicPaper *next;
} AcademicPaper;

// �������ѧ�ƾ����ṹ��
typedef struct Competition {
    char competitionName[MAX_NAME_LENGTH]; // ��������
    int teamSize;   //������������
    char competitionLevel[5]; // ��������,���Ҽ���ʡ����У��
    int rank;   // �񽱵ȼ���1��ʾһ�Ƚ���2��ʾ���Ƚ���3��ʾ���Ƚ�
    char competitionType;   //�������A�࣬B�࣬C��
    float gpa; // �ӷ�ֵ
    struct Competition *next;
} Competition;

// ѧ����Ϣ�ṹ��
typedef struct StudentInfo {
    char studentID[STUDENT_ID_LENGTH+1];// ѧ��
    char name[MAX_NAME_LENGTH];// ����
    char password[MAX_PASSWORD_LENGTH+2];// ����
    int classNumber;// �༶
    int grade;      //�꼶����һ����������������ģ�
    float gpa;  // ѧҵGPA
    float totalCredit;// ��ѧ��
    float qualityGpa;// ���ʼӷ�GPA
    float averageScore;// ��Ȩƽ����
    float totalGPA;// �ܼ���
    AcademicScoreNode *academicScores;// ѧҵ�ɼ�
    InnovationProject *innovationProjects;// ��ѧ�����´�ҵ�ƻ���Ŀ
    AcademicPaper *academicPapers;// ѧ������
    Competition *competitions;// �������ѧ�ƾ���
    struct StudentInfo *next;// ָ����һ��ѧ����ָ��
} StudentInfo;

// ����Ա��Ϣ�ṹ��
typedef struct AdminInfo {
    char username[MAX_NAME_LENGTH];// �û���
    char password[MAX_PASSWORD_LENGTH];// ����
    struct AdminInfo *next;
} AdminInfo;

// ȫ�ֱ���
StudentInfo *studentsList = NULL;
AdminInfo *adminsList=NULL;
int count = 0;//��¼����������
int isSaved = 0;//��¼�Ƿ񱣴浽�ļ�

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
void displaySelfMenu(StudentInfo *student);//ѧ���Լ���ʾ�˵�
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
void calculateAcademics(StudentInfo *student);//����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
float calculateSingleGPA(const float *score);//���㵥�ſγ̼���
void deleteStudentRecord(StudentInfo **head, char *studentID);//ɾ��ѧ����¼
void handleInputError(const char *message);//�����������
void displayAllStudents(StudentInfo *head);//��ʾ����ѧ����Ϣ�ſ�
void displayByClass(StudentInfo *head);//���༶��ʾѧ����Ϣ
void displayByGrade(StudentInfo *head);//���꼶��ʾѧ����Ϣ
void sortList(StudentInfo **head, int (*compare)(const StudentInfo *, const StudentInfo *),int len);//����ѧ������
int compareByStudentID(const StudentInfo *a, const StudentInfo *b);//��ѧ������
int compareByGPA(const StudentInfo *a, const StudentInfo *b);//��ѧҵGPA����
int compareByAverageScore(const StudentInfo *a, const StudentInfo *b);//����Ȩƽ��������
int compareByTotalGPA(const StudentInfo *a, const StudentInfo *b);//���ܼ�������
void addAdminAccount();//��ӹ���Ա�˺�
float calculatePaperGPA(AcademicPaper *paper);//����ѧ�����ļӷ�
float calculateCompetitionGPA(Competition *competition);//����������ѧ�ƾ����ӷ�
float calculateInnovationProjectGPA(InnovationProject *project);//�����ѧ�����´�ҵ�ƻ���Ŀ�ӷ�
void calculateQualityGPA(StudentInfo *student);//�������ʼӷ�GPA
void displayInnovationProjects(StudentInfo *student);//��ʾ��ѧ�����´�ҵ�ƻ���Ŀ
void displayAcademicPapers(StudentInfo *student);//��ʾѧ������
void displayCompetitions(StudentInfo *student);//��ʾ�������ѧ�ƾ���
void modifyInnovationProject(StudentInfo *student);//�޸Ĵ�ѧ�����´�ҵ�ƻ���Ŀ
void modifyAcademicPaper(StudentInfo *student);//�޸�ѧ������
void modifyCompetition(StudentInfo *student);//�޸ļ������ѧ�ƾ���
void deleteInnovationProject(StudentInfo *student);//ɾ����ѧ�����´�ҵ�ƻ���Ŀ
void deleteAcademicPaper(StudentInfo *student);//ɾ��ѧ������
void deleteCompetition(StudentInfo *student);//ɾ���������ѧ�ƾ���
void displayStudentMenu();//��ʾѧ���Ĳ˵�
void sortByGrade(StudentInfo **head);//���꼶����
void sortByClass(StudentInfo **head);//���༶����
int isStudentIDValid(const char *studentID);//�ж�ѧ���Ƿ�Ϸ�
int isGradeValid(int grade);//�ж��꼶�Ƿ�Ϸ�
int isClassValid(int classNumber);//�жϰ༶�Ƿ�Ϸ�
int isScoreValid(float score);//�жϷ����Ƿ�Ϸ�
int isPasswordValid(const char *password);//�ж������Ƿ�Ϸ�
int isDigit(char c);//�ж��Ƿ�Ϊ����
int isCreditValid(float credit);//�ж�ѧ���Ƿ�Ϸ�
int isCompetitionLevelValid(const char *competitionLevel);//�жϾ��������Ƿ�Ϸ�
int isCompetitionTypeValid(char competitionType);//�жϾ�������Ƿ�Ϸ�
int isProjectLevelValid(const char *projectLevel);//�ж���Ŀ�����Ƿ�Ϸ�
int isRankValid(int rank);//�жϻ񽱵ȼ��Ƿ�Ϸ�
int isLeaderOrSecondLeaderValid(int isLeaderOrSecondLeader);//�ж��Ƿ�Ϊ�������Ƿ�Ϸ�
void calculateJournalLevel(AcademicPaper *paper);//����ѧ�������ڿ�����
void freeTmpMemory(StudentInfo **head);//�ͷ���ʱѧ�������ڴ�

// ������
int main() {
    loadFromFile();//���ļ�����

    if(adminsList==NULL){
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
    }
    // ���û�ж�ȡ������Ա����ʼ������Ա��Ϣ,�û����������Ϊadmin

    loginSystem();// ����ϵͳ

    if(!isSaved)saveToFile();//���浽�ļ�

    freeMemory(&studentsList);//�ͷ��ڴ�
    freeAdminMemory(&adminsList);//�ͷ��ڴ�
    return 0;
}

// ��¼ϵͳ
void loginSystem() {
    char choice[MAX_NAME_LENGTH];
    printf("��ѡ���¼���ͣ�\n1. ѧ��\n2. ����Ա\n");
    scanf("%s", choice);
    //�ж�ѡ���Ƿ�Ϸ�
    while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0) {
        handleInputError("ѡ��ӦΪ1��2�����������롣\n");
        scanf("%s", choice);
    }
    if (strcmp(choice, "1") == 0) {
        studentLogin();
    } else if (strcmp(choice, "2") == 0) {
        adminLogin();
    }
}

// ѧ����¼
void studentLogin() {
    char studentID[STUDENT_ID_LENGTH], password[MAX_PASSWORD_LENGTH];
    printf("������ѧ�ź����룺\n");
    scanf("%s", studentID);//����ѧ��
    //�ж�ѧ���Ƿ����
    while (findStudentByID(studentID) == NULL) {
        handleInputError("ѧ�Ų����ڣ����������롣\n");
        scanf("%s", studentID);
    }
    scanf("%s", password);//��������
    //�ж������Ƿ�Ϸ�
    while (!isPasswordValid(password)) {
        handleInputError("����ӦΪ1-15λ���ֻ���ĸ�����������롣\n");
        scanf("%s", password);
    }
    // ��֤ѧ����Ϣ
    StudentInfo *student = findStudentByID(studentID);
    if (student && strcmp(student->password, password) == 0) {
        // ��¼�ɹ�
        printf("ѧ����¼�ɹ���\n");
        // ��ʾѧ���˵�
        displaySelfMenu(student);
    } else {
        // ��¼ʧ��
        printf("ѧ�Ż��������\n");
        count++;
        if (count == 3) {
            printf("�������������࣬�˳�ϵͳ��\n");
            exit(1);
        }else{
            studentLogin();
        }
    }
}

// ����Ա��¼
void adminLogin() {
    char username[MAX_NAME_LENGTH];
    printf("���������Ա�û�����\n");
    scanf("%s", username);
    // �ж��û����Ƿ����
    while (findAdminByUsername(username) == NULL) {
        handleInputError("�û��������ڣ����������롣\n");
        scanf("%s", username);
    }
    char password[MAX_PASSWORD_LENGTH];
    printf("���������Ա���룺\n");
    scanf("%s", password);
    //�ж������Ƿ�Ϸ�
    while (!isPasswordValid(password)) {
        handleInputError("����ӦΪ1-15λ���ֻ���ĸ�����������롣\n");
        scanf("%s", password);
    }
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
        count++;
        if (count == 3) {
            printf("�������������࣬�˳�ϵͳ��\n");
            exit(1);
        }else{
            adminLogin();
        }
    }
}

// ���ѧ���˺�
void addStudentAccount() {
    StudentInfo *newStudent = createStudent();
    // ����GPA
    calculateAcademics(newStudent);
    // ����ѧ���б�
    insertStudent(&studentsList, newStudent);
    printf("ѧ���˺Ŵ����ɹ���\n");
}

// ��ӿγ�
void addCourse(StudentInfo *student) {
    AcademicScoreNode *newCourse = createAcademicScore();
    insertAcademicScore(student, newCourse);
    // ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
    calculateAcademics(student);
    printf("�γ���ӳɹ���\n");
}

// ������ʼӷ���Ŀ
void addQualityProject(StudentInfo *student) {
    char choice[MAX_NAME_LENGTH];
    do{
        printf("��ѡ��ӷ���Ŀ���ͣ�\n");
        printf("1. ��ѧ�����´�ҵ�ƻ���Ŀ\n");
        printf("2. ѧ������\n");
        printf("3. �������ѧ�ƾ���\n");
        printf("4. ����\n");
        scanf("%s", choice);
        //�ж�ѡ���Ƿ�Ϸ�
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0) {
            handleInputError("ѡ��ӦΪ1-4֮����������������롣\n");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            InnovationProject *newProject = createInnovationProject();
            insertInnovationProject(student, newProject);
            // ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
            calculateAcademics(student);
            printf("��ѧ�����´�ҵ�ƻ���Ŀ��ӳɹ���\n");
        } else if (strcmp(choice, "2") == 0) {
            AcademicPaper *newPaper = createAcademicPaper();
            insertAcademicPaper(student, newPaper);
            // ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
            calculateAcademics(student);
            printf("ѧ��������ӳɹ���\n");
        } else if (strcmp(choice, "3") == 0) {
            Competition *newCompetition = createCompetition();
            insertCompetition(student, newCompetition);
            // ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
            calculateAcademics(student);
            printf("�������ѧ�ƾ�����ӳɹ���\n");
        } else if (strcmp(choice, "4") == 0) {
            printf("���ء�\n");
        }
    }while(strcmp(choice, "4") != 0);
}

// �༭ѧ���ɼ���ɾ��ѧ��
void modifyScoreMenu() {
    char studentID[STUDENT_ID_LENGTH];
    printf("������ѧ��ѧ��: ");
    scanf("%s", studentID);
    //�ж�ѧ���Ƿ�Ϸ�
    while (!isStudentIDValid(studentID)) {
        handleInputError("ѧ��ӦΪ8λ���֣����������롣\n");
        scanf("%s", studentID);
    }
    StudentInfo *student = findStudentByID(studentID);
    if (student != NULL) {
        char choice[MAX_NAME_LENGTH];
        do{
            printf("��ѡ���޸����ͣ�\n");
            printf("1. �޸�ѧҵ�ɼ�\n");
            printf("2. ���ѧҵ�ɼ�\n");
            printf("3. �޸����ʼӷ�\n");
            printf("4. ������ʼӷ�\n");
            printf("5. �޸��꼶\n");
            printf("6. �޸İ༶\n");
            printf("7. ɾ��ѧ��\n");
            printf("8. ����\n");
            scanf("%s", choice);
            //�ж�ѡ���Ƿ�Ϸ�
            while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0 && strcmp(choice, "6") != 0 && strcmp(choice, "7") != 0 && strcmp(choice, "8") != 0) {
                handleInputError("ѡ��ӦΪ1-8֮����������������롣\n");
                scanf("%s", choice);
            }
            if (strcmp(choice, "1") == 0) {
                modifyAcademicScore(student);
            } else if (strcmp(choice, "2") == 0) {
                addCourse(student);
            } else if (strcmp(choice, "3") == 0) {
                modifyQualityScore(student);
            } else if (strcmp(choice, "4") == 0) {
                addQualityProject(student);
            } else if (strcmp(choice, "5") == 0) {
                int grade;
                printf("���������꼶��");
                scanf("%d", &grade);
                //�ж��꼶�Ƿ�Ϸ�
                while (!isGradeValid(grade)) {
                    handleInputError("�꼶ӦΪ1-4֮����������������롣\n");
                    scanf("%d", &grade);
                }
                student->grade = grade;
                printf("�꼶�޸ĳɹ���\n");
            } else if (strcmp(choice, "6") == 0) {
                int classNumber;
                printf("�������°༶��");
                scanf("%d", &classNumber);
                //�жϰ༶�Ƿ�Ϸ�
                while (!isClassValid(classNumber)) {
                    handleInputError("�༶ӦΪ1-20֮����������������롣\n");
                    scanf("%d", &classNumber);
                }
                student->classNumber = classNumber;
                printf("�༶�޸ĳɹ���\n");
            } else if (strcmp(choice, "7") == 0) {
                deleteStudentRecord(&studentsList, studentID);
                printf("ѧ��ɾ���ɹ���\n");
            } else if (strcmp(choice, "8") == 0) {
                printf("���ء�\n");
            }
            // ����GPA����ѧ�֡���Ȩƽ���֡��ܼ���
            calculateAcademics(student);
            printf("�ɼ��޸ĳɹ���\n");
        }while(strcmp(choice, "8") != 0);
    } else {
        printf("δ�ҵ�ѧ����Ϣ��\n");
    }
}

// ����ѧ��
void sortStudentsMenu() {
    // ����GPA��������׼��ѧ���б��������
    char choice[MAX_NAME_LENGTH];
    do{
        printf("��ѡ������Χ��\n");
        printf("1. ���꼶��Χ����\n");
        printf("2. ���༶��Χ����\n");
        printf("3. ����\n");
        scanf("%s", choice);
        //�ж�ѡ���Ƿ�Ϸ�
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0) {
            handleInputError("ѡ��ӦΪ1-3֮����������������롣\n");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            sortByGrade(&studentsList);
            printf("���꼶������ɡ�\n");
        } else if (strcmp(choice, "2") == 0) {
            sortByClass(&studentsList);
            printf("���༶������ɡ�\n");
        } else if (strcmp(choice, "3") == 0) {
            printf("���ء�\n");
        }
    }while(strcmp(choice, "3") != 0);
}

// ��ʾָ��ѧ����Ϣ�ſ�
void displayStudentInfo(StudentInfo *student) {
    // ��ʾѧ���ĳɼ��ͼӷ���Ϣ
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    printf("ѧ��: %s\n", student->studentID);
    printf("����: %s\n", student->name);
    printf("�༶: %d\n", student->classNumber);
    printf("�꼶: %d\n", student->grade);
    printf("�ѻ���ѧ��: %.2f\n", student->totalCredit);
    printf("ѧҵGPA: %f\n", student->gpa);
    printf("���ʼӷ�GPA: %.2f\n", student->qualityGpa);
    printf("��Ȩƽ����: %.2f\n", student->averageScore);
    printf("�ܼ���: %.2f\n", student->totalGPA);
    printf("ѧ����Ϣ��ʾ��ɡ�\n");
    char choice[MAX_NAME_LENGTH];
    do{
        printf("�Ƿ�Ҫ������ʾ��ϸ��Ϣ��\n");
        printf("1. ��ʾ����ѧҵ�ɼ�\n");
        printf("2. ��ʾ�������ʼӷ�\n");
        printf("3. ����\n");
        scanf("%s", choice);
        //�ж�ѡ���Ƿ�Ϸ�
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0) {
            handleInputError("ѡ��ӦΪ1-3֮����������������롣\n");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            displayAcademicScore(student);//��ʾѧҵ�ɼ�
        } else if (strcmp(choice, "2") == 0) {
            displayQualityScore(student);//��ʾ���ʼӷ�
        } else if (strcmp(choice, "3") == 0) {
            printf("���ء�\n");
        }
    }while(strcmp(choice, "3") != 0);
}

// ��ʾ����Ա�˵�
void displayAdminMenu(AdminInfo *admin) {
    char choice[MAX_NAME_LENGTH];
    do {
        printf("����Ա�˵���\n");
        printf("1. �����ѧ��\n");
        printf("2. ��ӹ���Ա�˺�\n");
        printf("3. �༭ѧ���ɼ���ɾ��ѧ��\n");
        printf("4. ����ͬҪ������ѧ��\n");
        printf("5. ����ͬ��Χ��ʾѧ����Ϣ\n");
        printf("6. ����ѧ��\n");
        printf("7. ���浽�ļ�\n");
        printf("8. ���ļ�����\n");
        printf("9. ��������\n");
        printf("10. �˳�\n");
        printf("��ѡ�������");
        scanf("%s", choice);
        //�ж�ѡ���Ƿ�Ϸ�
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0 && strcmp(choice, "6") != 0 && strcmp(choice, "7") != 0 && strcmp(choice, "8") != 0 && strcmp(choice, "9") != 0 && strcmp(choice, "10") != 0) {
            handleInputError("ѡ��ӦΪ1-10֮����������������롣\n");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            addStudentAccount();
        } else if (strcmp(choice, "2") == 0) {
            addAdminAccount();
        } else if (strcmp(choice, "3") == 0) {
            modifyScoreMenu();
        } else if (strcmp(choice, "4") == 0) {
            sortStudentsMenu();
        } else if (strcmp(choice, "5") == 0) {
            displayStudentMenu();
        } else if (strcmp(choice, "6") == 0) {
            char studentID[STUDENT_ID_LENGTH];
            printf("������ѧ��ѧ��: ");
            scanf("%s", studentID);
            //�ж�ѧ���Ƿ�Ϸ�
            while (!isStudentIDValid(studentID)) {
                handleInputError("ѧ��ӦΪ8λ���֣����������롣\n");
                scanf("%s", studentID);
            }
            StudentInfo *student = findStudentByID(studentID);
            displayStudentInfo(student);
        } else if (strcmp(choice, "7") == 0) {
            saveToFile();
        } else if (strcmp(choice, "8") ==0) {
            loadFromFile();
        } else if (strcmp(choice, "9") == 0) {
                char newPassword[MAX_PASSWORD_LENGTH];
                printf("�����������룺");
                scanf("%s", newPassword);
                //�ж����볤���Ƿ�Ϸ�
                while (!isPasswordValid(newPassword)) {
                 handleInputError("���볤��ӦΪ1-15λ�����������롣\n");
                 scanf("%s", newPassword);
                }
                strcpy(admin->password, newPassword);
                printf("�����޸ĳɹ���\n");
        } else if (strcmp(choice, "10") == 0) {
                printf("����Ա�˳���¼��\n");
        }
    } while (strcmp(choice, "10") != 0);
}

//��ʾѧ���˵�
void displaySelfMenu(StudentInfo *student) {
    char choice[MAX_NAME_LENGTH];
    do {
        printf("ѧ���˵���\n");
        printf("1. �鿴�ɼ�\n");
        printf("2. �鿴���ʼӷ���Ŀ\n");
        printf("3. �޸�����\n");
        printf("4. �˳�\n");
        printf("��ѡ�������");
        scanf("%s", choice);
        //�ж�ѡ���Ƿ�Ϸ�
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0) {
            handleInputError("ѡ��ӦΪ1-4֮����������������롣\n");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            displayAcademicScore(student);//��ʾѧҵ�ɼ�
        } else if (strcmp(choice, "2") == 0) {
            displayQualityScore(student);//��ʾ���ʼӷ�
        } else if (strcmp(choice, "3") == 0) {
            char newPassword[MAX_PASSWORD_LENGTH];
            printf("�����������룺");
            scanf("%s", newPassword);
            //�ж����볤���Ƿ�Ϸ�
            while (!isPasswordValid(newPassword)) {
                handleInputError("���볤��ӦΪ1-15λ�����������롣\n");
                scanf("%s", newPassword);
            }
            strcpy(student->password, newPassword);
            printf("�����޸ĳɹ���\n");
        } else if (strcmp(choice, "4") == 0) {
            printf("ѧ���˳���¼��\n");
        }
    } while (strcmp(choice, "4") != 0);
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
void calculateAcademics(StudentInfo *student) {
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
    if(totalCredit>0){
        student->averageScore = totalScore / totalCredit;
    }else {
        student->averageScore = 0.00f;
    }
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
    newStudent->grade = 1; // ȱʡֵѧ���꼶Ϊ1
    strcpy(newStudent->password, "123456"); // Ĭ������Ϊ123456
    // ����ѧ����Ϣ
    printf("������ѧ����Ϣ��\n");
    printf("ѧ��: ");
    scanf("%s", newStudent->studentID);
    // �ж�ѧ���Ƿ���8λ����
    while (!isStudentIDValid(newStudent->studentID)) {
        handleInputError("ѧ��ӦΪ8λ���֣����������롣\n");
        scanf("%s", newStudent->studentID);
    }
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
    scanf("%d", &(newStudent->classNumber));
    //�жϰ༶�Ƿ�Ϸ�
    while (!isClassValid(newStudent->classNumber)) {
        handleInputError("�༶ӦΪ����0���������������롣\n");
        scanf("%d", &newStudent->classNumber);
    }

    printf("�꼶: ");
    scanf("%d", &newStudent->grade);
    //�ж��꼶�Ƿ�Ϸ�
    while (!isGradeValid(newStudent->grade)) {
        handleInputError("�꼶ӦΪ1-4֮�����(1.��һ 2.��� 3.���� 4.����)�����������롣\n");
        scanf("%d", &newStudent->grade);
    }

    //¼��ѧҵ�ɼ�
    char choice1[MAX_NAME_LENGTH];
    printf("�Ƿ�����¼��γ̼�¼��(1. �� 2. ��): ");
    scanf("%s", choice1);
    //�ж�ѡ���Ƿ�Ϸ�
    while (strcmp(choice1, "1") != 0 && strcmp(choice1, "2") != 0) {
        handleInputError("ѡ��ӦΪ1��2�����������롣\n");
        scanf("%s", choice1);
    }
    if(strcmp(choice1, "1") == 0){
        int numCourses;
        printf("������Ҫ����Ŀγ�������");
        scanf("%d", &numCourses);
        for (int i = 0; i < numCourses; i++) {
            printf("�������%d�ſγ���Ϣ��\n", i + 1);
            addCourse(newStudent);
        }
        calculateAcademics(newStudent);
    }

    //¼�����ʼӷ���Ŀ
    char choice2[MAX_NAME_LENGTH];
    printf("�Ƿ�����¼�����ʼӷ���Ŀ��¼��(1. �� 2. ��): ");
    scanf("%s", choice2);
    //�ж�ѡ���Ƿ�Ϸ�
    while (strcmp(choice2, "1") != 0 && strcmp(choice2, "2") != 0) {
        handleInputError("ѡ��ӦΪ1��2�����������롣\n");
        scanf("%s", choice2);
    }
    if(strcmp(choice2, "1") == 0){
        //¼�����ʼӷ���Ŀ
        addQualityProject(newStudent);
        calculateQualityGPA(newStudent);
        calculateAcademics(newStudent);
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
    newCourse->gpa = calculateSingleGPA(&newCourse->score); // ��ʼ���γ̼���
    printf("������γ���Ϣ��\n");
    printf("�γ�����: ");
    scanf("%s", newCourse->courseName);
    printf("����: ");
    scanf("%f", &newCourse->score);
    // �жϷ����Ƿ�Ϸ�
    while (!isScoreValid(newCourse->score)) {
        handleInputError("����ӦΪ0-100֮����������������롣\n");
        scanf("%f", &newCourse->score);
    }
    printf("ѧ��: ");
    scanf("%f", &newCourse->credit);
    // �ж�ѧ���Ƿ�Ϸ�
    while (!isCreditValid(newCourse->credit)) {
        handleInputError("ѧ��ӦΪС��8�������������롣\n");
        scanf("%f", &newCourse->credit);
    }
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
    printf("�Ƿ�Ϊ��Ŀ������(0.�� 1.��һ������ 2.�ڶ�������): ");
    scanf("%d", &newProject->isLeaderOrSecondLeader);
    // �ж��Ƿ�Ϊ��Ŀ�������Ƿ�Ϸ�
    while (!isLeaderOrSecondLeaderValid(newProject->isLeaderOrSecondLeader)) {
        handleInputError("�Ƿ�Ϊ��Ŀ������ӦΪ0-2֮����������������롣\n");
        scanf("%d", &newProject->isLeaderOrSecondLeader);
    }
    printf("��Ŀ����(���Ҽ���ʡ����У��): ");
    scanf("%s", newProject->projectLevel);
    // �ж���Ŀ�����Ƿ�Ϸ�
    while (!isProjectLevelValid(newProject->projectLevel)) {
        handleInputError("��Ŀ����ӦΪ���Ҽ���ʡ����У�������������롣\n");
        scanf("%s", newProject->projectLevel);
    }
    printf("�Ƿ����(1. �� 2. ��): ");
    scanf("%d", &newProject->isFinished);
    // �ж��Ƿ�����Ƿ�Ϸ�
    while (newProject->isFinished != 1 && newProject->isFinished != 2) {
        handleInputError("�Ƿ����ӦΪ1��2�����������롣\n");
        scanf("%d", &newProject->isFinished);
    }
    newProject->gpa = calculateInnovationProjectGPA(newProject);
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
    // �ж��Ƿ�Ϊ��һ�����Ƿ�Ϸ�
    while (newPaper->isFirstAuthor != 1 && newPaper->isFirstAuthor != 2) {
        handleInputError("�Ƿ�Ϊ��һ����ӦΪ1��2�����������롣\n");
        scanf("%d", &newPaper->isFirstAuthor);
    }
    newPaper->journalLevel = 0; // Ĭ���ڿ������ʼ��Ϊ0
    calculateJournalLevel(newPaper); // �����ڿ�����
    newPaper->gpa = calculatePaperGPA(newPaper);
    newPaper->next = NULL; //���ָ���ʼ��ΪNULL
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
    // �жϾ��������Ƿ�Ϸ�
    while (!isCompetitionLevelValid(newCompetition->competitionLevel)) {
        handleInputError("��������ӦΪ���Ҽ���ʡ����У�������������롣\n");
        scanf("%s", newCompetition->competitionLevel);
    }
    printf("�񽱵ȼ�(1. һ�Ƚ� 2. ���Ƚ� 3. ���Ƚ�): ");
    scanf("%d", &newCompetition->rank);
    // �жϻ񽱵ȼ��Ƿ�Ϸ�
    while (!isRankValid(newCompetition->rank)) {
        handleInputError("�񽱵ȼ�ӦΪ1-3֮����������������롣\n");
        scanf("%d", &newCompetition->rank);
    }
    printf("�������(A�࣬B�࣬C��): ");
    scanf("%c", &newCompetition->competitionType);
    // �жϾ�������Ƿ�Ϸ�
    while (!isCompetitionTypeValid(newCompetition->competitionType)) {
        handleInputError("�������ӦΪA�࣬B�࣬C�࣬���������롣\n");
        scanf("%c", &newCompetition->competitionType);
    }
    newCompetition->gpa = calculateCompetitionGPA(newCompetition);
    newCompetition->next = NULL; //���ָ���ʼ��ΪNULL
    // �����´�����Competitionָ��
    return newCompetition;
}

// ��ѧ�Ų���ָ��ѧ��
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
    AdminInfo *currentAdmin = adminsList;
    while (currentAdmin != NULL) {
        if (strcmp(currentAdmin->username, username) == 0) {
            // ����ҵ�ƥ����û��������ص�ǰ����Ա����Ϣ
            return currentAdmin;
        }
        currentAdmin = currentAdmin->next;
    }
    // ���û���ҵ�ƥ����û���������NULL
    return NULL;
}

// �༭ѧ��ѧҵ�ɼ�
void modifyAcademicScore(StudentInfo *student) {
    // �޸�ѧҵ�ɼ�
    int choice;
    do{
        printf("��ѡ���޸����ͣ�\n");
        printf("1. �޸Ŀγ�����\n");
        printf("2. �޸ķ���\n");
        printf("3. �޸�ѧ��\n");
        printf("4. ɾ���γ�\n");
        printf("5. ������һ���˵�\n");
        scanf("%d", &choice);
        //�ж�ѡ���Ƿ�Ϸ�
        while (choice < 1 || choice > 5) {
            handleInputError("ѡ��ӦΪ1-5֮����������������롣\n");
            scanf("%d", &choice);
        }
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
                        printf("�ҵ��γ�: %s, ��ǰ����: %f\n", currentScore->courseName, currentScore->score);
                        printf("�������µķ���: ");
                        scanf("%f", &currentScore->score);
                        //�жϷ����Ƿ�Ϸ�
                        while (!isScoreValid(currentScore->score)) {
                            handleInputError("����ӦΪ0-100֮����������������롣\n");
                            scanf("%f", &currentScore->score);
                        }
                        currentScore->gpa = calculateSingleGPA(&currentScore->score);//���¼��㵱ǰ�γ̼���
                        calculateAcademics(student);//���¼���GPA\��ѧ��\��Ȩƽ����\�ܼ���
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
                        //�ж�ѧ���Ƿ�Ϸ�
                        while (!isCreditValid(currentCredit->credit)) {
                            handleInputError("ѧ��ӦΪС��8�������������롣\n");
                            scanf("%2f", &currentCredit->credit);
                        }
                        calculateAcademics(student);  //���¼���GPA\��ѧ��\��Ȩƽ����\�ܼ���
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
                        calculateAcademics(student);  //���¼���GPA\��ѧ��\��Ȩƽ����\�ܼ���
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
                return;
            }
            default: {
                printf("��Ч��ѡ�\n");
            }
        }
    }while (choice != 5);
}

// �༭ѧ�����ʼӷ�
void modifyQualityScore(StudentInfo *student) {
    // �޸����ʼӷ�
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    char choice[MAX_NAME_LENGTH];
    do{
        printf("��ѡ���޸����ͣ�\n");
        printf("1. �޸Ĵ�ѧ�����´�ҵ��Ŀ\n");
        printf("2. ɾ����ѧ�����´�ҵ��Ŀ\n");
        printf("3. �޸�ѧ������\n");
        printf("4. ɾ��ѧ������\n");
        printf("5. �޸ļ������ѧ�ƾ���\n");
        printf("6. ɾ���������ѧ�ƾ���\n");
        printf("7. ������һ���˵�\n");
        scanf("%s", choice);
        // �ж�ѡ���Ƿ�Ϸ�
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0 && strcmp(choice, "6") != 0 && strcmp(choice, "7") != 0) {
            handleInputError("ѡ��ӦΪ1-7֮����������������롣\n");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            // �޸Ĵ�ѧ�����´�ҵ��Ŀ
            modifyInnovationProject(student);
        } else if (strcmp(choice, "2") == 0) {
            // ɾ����ѧ�����´�ҵ��Ŀ
            deleteInnovationProject(student);
        } else if (strcmp(choice, "3") == 0) {
            // �޸�ѧ������
            modifyAcademicPaper(student);
        } else if (strcmp(choice, "4") == 0) {
            // ɾ��ѧ������
            deleteAcademicPaper(student);
        } else if (strcmp(choice, "5") == 0) {
            // �޸ļ������ѧ�ƾ���
            modifyCompetition(student);
        } else if (strcmp(choice, "6") == 0) {
            // ɾ���������ѧ�ƾ���
            deleteCompetition(student);
        } else if (strcmp(choice, "7") == 0) {
            // ������һ���˵�
            printf("������һ���˵���\n");
        }
    }while (strcmp(choice, "7") != 0);
}

// ��ʾָ��ѧ����ȫ��ѧҵ�ɼ�
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

// ��ʾָ��ѧ����ȫ�����ʼӷ�
void displayQualityScore(StudentInfo *student) {
    // ��ʾѧ�������ʼӷ�
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    displayInnovationProjects(student);//��ʾ��ѧ�����´�ҵ�ƻ���Ŀ
    displayAcademicPapers(student);//��ʾѧ������
    displayCompetitions(student);//��ʾ�������ѧ�ƾ���
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
void displayAllStudents(StudentInfo *head) {
    // ��ʾ����ѧ������Ϣ
    if (head == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    StudentInfo *current = head;
    while (current != NULL) {
        printf("ѧ��: %s\n", current->studentID);
        printf("����: %s\n", current->name);
        printf("�꼶: %d\n", current->grade);
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
void sortList(StudentInfo **headRef, int (*compare)(const StudentInfo *, const StudentInfo *),int len) {
    // ʹ��ð�������ѧ�������������
    int i, j;
    StudentInfo *current;
    StudentInfo *next;
    for (i = 0; i < len - 1; i++) {
        current = *headRef;
        next = current->next;
        for (j = 0; j < len - i - 1; j++) {
            if (compare(current, next) > 0) {
                // ���������ڵ������
                char tmpID[STUDENT_ID_LENGTH+1];
                strcpy(tmpID, current->studentID);
                strcpy(current->studentID, next->studentID);
                strcpy(next->studentID, tmpID);
                char tmpName[MAX_NAME_LENGTH];
                strcpy(tmpName, current->name);
                strcpy(current->name, next->name);
                strcpy(next->name, tmpName);
                char tmpPassword[MAX_PASSWORD_LENGTH+2];
                strcpy(tmpPassword, current->password);
                strcpy(current->password, next->password);
                strcpy(next->password, tmpPassword);
                int tmpClassNumber = current->classNumber;
                current->classNumber = next->classNumber;
                next->classNumber = tmpClassNumber;
                int tmpGrade = current->grade;
                current->grade = next->grade;
                next->grade = tmpGrade;
                float tmpGPA = current->gpa;
                current->gpa = next->gpa;
                next->gpa = tmpGPA;
                float tmpTotalCredit = current->totalCredit;
                current->totalCredit = next->totalCredit;
                next->totalCredit = tmpTotalCredit;
                float tmpQualityGpa = current->qualityGpa;
                current->qualityGpa = next->qualityGpa;
                next->qualityGpa = tmpQualityGpa;
                float tmpAverageScore = current->averageScore;
                current->averageScore = next->averageScore;
                next->averageScore = tmpAverageScore;
                float tmpTotalGPA = current->totalGPA;
                current->totalGPA = next->totalGPA;
                next->totalGPA = tmpTotalGPA;
                AcademicScoreNode *tmpAcademicScores = current->academicScores;
                current->academicScores = next->academicScores;
                next->academicScores = tmpAcademicScores;
                InnovationProject *tmpInnovationProjects = current->innovationProjects;
                current->innovationProjects = next->innovationProjects;
                next->innovationProjects = tmpInnovationProjects;
                AcademicPaper *tmpAcademicPapers = current->academicPapers;
                current->academicPapers = next->academicPapers;
                next->academicPapers = tmpAcademicPapers;
                Competition *tmpCompetitions = current->competitions;
                current->competitions = next->competitions;
                next->competitions = tmpCompetitions;
            }
            current = current->next;
            next = next->next;
        }
    }
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
        handleInputError("�û����Ѵ��ڣ�����������: ");
        scanf("%s", newAdmin->username);
    }

    printf("���������Ա����: ");
    scanf("%s", newAdmin->password);
    // ��������Ƿ�Ϸ�
    while (!isPasswordValid(newAdmin->password)) {
        handleInputError("����ӦΪ1-15λ�ַ�������������: ");
        scanf("%s", newAdmin->password);
    }

    // ���¹���Ա��ӵ�����ͷ��
    newAdmin->next = adminsList;
    adminsList = newAdmin;

    printf("����Ա�˺���ӳɹ���\n");
}

// ���浽�ļ�
void saveToFile() {
    FILE *file = fopen("student.txt", "w");
    if (file == NULL) {
        handleInputError("���ļ�ʧ��");
        return;
    }

    StudentInfo *currentStudent = studentsList;
    while (currentStudent != NULL) {
        // ѧ����¼��ʼ
        fprintf(file, "%s\n", STUDENT_START);

//        char studentID[8];// ѧ��
//        char name[15];// ����
//        char password[15];// ����
//        int classNumber;// �༶
//        float gpa;  // ѧҵGPA
//        float totalCredit;// ��ѧ��
//        float qualityGpa;// ���ʼӷ�GPA
//        float averageScore;// ��Ȩƽ����
//        float totalGPA;// �ܼ���

        // д��ѧ��������Ϣ
        fprintf(file, "StudentID: %s\n", currentStudent->studentID);// ѧ��
        fprintf(file, "Name: %s\n", currentStudent->name);// ����
        fprintf(file, "Password: %s\n", currentStudent->password);// ����
        fprintf(file, "Grade: %d\n", currentStudent->grade);// �꼶
        fprintf(file, "ClassNumber: %d\n", currentStudent->classNumber);// �༶
        fprintf(file, "GPA: %f\n", currentStudent->gpa);// ѧҵGPA
        fprintf(file, "TotalCredit: %.2f\n", currentStudent->totalCredit);// ��ѧ��
        fprintf(file, "QualityGPA: %.2f\n", currentStudent->qualityGpa);// ���ʼӷ�GPA
        fprintf(file, "AverageScore: %f\n", currentStudent->averageScore);// ��Ȩƽ����
        fprintf(file, "TotalGPA: %f\n", currentStudent->totalGPA);// �ܼ���

//        char courseName[MAX_NAME_LENGTH];
//        float score;    // ����
//        float credit; // ѧ��
//        float gpa; // ���ſγ̼���
        // ѧҵ�ɼ�����
        if (currentStudent->academicScores != NULL) {
            fprintf(file, "%s\n", SCORE_START);
            AcademicScoreNode *currentScore = currentStudent->academicScores;
            while (currentScore != NULL) {
                fprintf(file, "CourseName: %s\n", currentScore->courseName);
                fprintf(file, "Score: %f\n", currentScore->score);
                fprintf(file, "Credit: %.2f\n", currentScore->credit);
                fprintf(file, "GPA: %.2f\n", currentScore->gpa);
                currentScore = currentScore->next;
            }
            fprintf(file, "%s\n", SCORE_END);
        }

//        char projectName[MAX_NAME_LENGTH]; // ��Ŀ����
//        float gpa; // �ӷ�ֵ
//        int isLeaderOrSecondLeader;// �Ƿ�Ϊ��Ŀ������,�Ƿ�Ϊ�ڶ������ˣ����Ҽ����������Ŀ�����˼�0.1 GPA���ڶ�����0.05 GPA
//        char projectLevel[10];// ��Ŀ���𣬹��Ҽ���ʡ����У��
//        int isFinished;    // �Ƿ����
        // ��ѧ�����´�ҵ�ƻ���Ŀ����
        if (currentStudent->innovationProjects != NULL) {
            fprintf(file, "%s\n", PROJECT_START);
            InnovationProject *currentProject = currentStudent->innovationProjects;
            if(currentProject != NULL) {
                fprintf(file, "ProjectName: %s\n", currentProject->projectName);
                fprintf(file, "GPA: %.2f\n", currentProject->gpa);
                fprintf(file, "LeaderOrSecondLeader: %d\n", currentProject->isLeaderOrSecondLeader);
                fprintf(file, "ProjectLevel: %s\n", currentProject->projectLevel);
                fprintf(file, "IsFinished: %d\n", currentProject->isFinished);
            }
            fprintf(file, "%s\n", PROJECT_END);
        }

//        char title[MAX_NAME_LENGTH]; // ���ı���
//        char journalName[MAX_NAME_LENGTH]; // �ڿ�����
//        int isFirstAuthor; // �Ƿ�Ϊ��һ����
//
//        //�ڿ����𣬶�����0.4��һ���ڿ���0.2 GPA�������ڿ���0.1 GPA�������ڿ���0.05 GPA���ļ��ڿ���0.02 GPA���弶�ڿ���0.01 GPA
//        int journalLevel;
//
//        float gpa; // �ӷ�ֵ
        // ѧ����������
        if (currentStudent->academicPapers != NULL) {
            fprintf(file, "%s\n", PAPER_START);
            AcademicPaper *currentPaper = currentStudent->academicPapers;
            while (currentPaper != NULL) {
                fprintf(file, "Title: %s\n", currentPaper->title);
                fprintf(file, "JournalName: %s\n", currentPaper->journalName);
                fprintf(file, "IsFirstAuthor: %d\n", currentPaper->isFirstAuthor);
                fprintf(file, "JournalLevel: %d\n", currentPaper->journalLevel);
                fprintf(file, "GPA: %.2f\n", currentPaper->gpa);
                currentPaper = currentPaper->next;
            }
            fprintf(file, "%s\n", PAPER_END);
        }

//        char competitionName[MAX_NAME_LENGTH]; // ��������
//        int teamSize;   //������������
//        char competitionLevel[10]; // ��������,���Ҽ���ʡ����У��
//        int rank;   // �񽱵ȼ���1��ʾһ�Ƚ���2��ʾ���Ƚ���3��ʾ���Ƚ�
//        char competitionType;   //�������A�࣬B�࣬C��
//        float gpa; // �ӷ�ֵ
        // �������ѧ�ƾ�������
        if (currentStudent->competitions != NULL) {
            fprintf(file, "%s\n", COMPETITION_START);
            Competition *currentCompetition = currentStudent->competitions;
            while (currentCompetition != NULL) {
                fprintf(file, "CompetitionName: %s\n", currentCompetition->competitionName);
                fprintf(file, "TeamSize: %d\n", currentCompetition->teamSize);
                fprintf(file, "CompetitionLevel: %s\n", currentCompetition->competitionLevel);
                fprintf(file, "Rank: %d\n", currentCompetition->rank);
                fprintf(file, "CompetitionType: %c\n", currentCompetition->competitionType);
                fprintf(file, "GPA: %.2f\n", currentCompetition->gpa);
                currentCompetition = currentCompetition->next;
            }
            fprintf(file, "%s\n", COMPETITION_END);
        }

        // ��ǰѧ����¼����
        fprintf(file, "%s\n", STUDENT_END);

        currentStudent = currentStudent->next;
    }
    fclose(file);//�ر��ļ�

    // �������Ա��Ϣ
    FILE *adminFile = fopen("admin.txt", "w");
    if (adminFile == NULL) {
        handleInputError("���ļ�ʧ��");
        return;
    }
    AdminInfo *currentAdmin = adminsList;
    while (currentAdmin != NULL) {
        fprintf(adminFile, "%s\n", ADMIN_START);
        fprintf(adminFile, "Username: %s\n", currentAdmin->username);
        fprintf(adminFile, "Password: %s\n", currentAdmin->password);
        fprintf(adminFile, "%s\n", ADMIN_END);
        currentAdmin = currentAdmin->next;
    }
    fclose(adminFile);//�ر��ļ�
    isSaved = 1;//���ñ����־
    printf("����ɹ���\n");
}

// ���ļ�����
void loadFromFile() {
    FILE* file = fopen("student.txt", "r");
    if (file == NULL) {
        handleInputError("���ļ�ʧ��");
        return;
    }

    freeMemory(&studentsList); // ���ѧ������

    char buffer[MAX_NAME_LENGTH];// ���ڴ洢��ȡ��ÿ������
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, STUDENT_START, strlen(STUDENT_START)) == 0) {
            // �����µ�ѧ���ڵ�
            StudentInfo* newStudent = (StudentInfo*)malloc(sizeof(StudentInfo));
            if (newStudent == NULL) {
                handleInputError("�ڴ����ʧ��");
                break;
            }
            memset(newStudent, 0, sizeof(StudentInfo));
            newStudent->next = NULL;
            newStudent->academicScores = NULL;
            newStudent->innovationProjects = NULL;
            newStudent->academicPapers = NULL;
            newStudent->competitions = NULL;

            // ��ȡѧ��������Ϣ
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "StudentID: %s", newStudent->studentID);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "Name: %s", newStudent->name);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "Password: %s", newStudent->password);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "Grade: %d", &newStudent->grade);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "ClassNumber: %d", &newStudent->classNumber);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "GPA: %f", &newStudent->gpa);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "TotalCredit: %2f", &newStudent->totalCredit);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "QualityGPA: %f", &newStudent->qualityGpa);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "AverageScore: %f", &newStudent->averageScore);
            fgets(buffer, sizeof(buffer), file);
            sscanf(buffer, "TotalGPA: %f", &newStudent->totalGPA);

            // ���½ڵ���ӵ�ѧ��������
            insertStudent(&studentsList, newStudent);
            while(fgets(buffer, sizeof(buffer), file))
            {
                // ��ȡѧҵ�ɼ�����������##ScoreStart##ʱ��ʼ�ɼ�¼�룬����##ScoreEnd##ʱ����
                if (strncmp(buffer, SCORE_START, strlen(SCORE_START)) == 0) {
                    while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, SCORE_END, strlen(SCORE_END)) != 0) {
                        AcademicScoreNode *newScore = (AcademicScoreNode *) malloc(sizeof(AcademicScoreNode));
                        if (newScore == NULL) {
                            handleInputError("�ڴ����ʧ��");
                            break;
                        }
                        memset(newScore, 0, sizeof(AcademicScoreNode));
                        //fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "CourseName: %s", newScore->courseName);

                        fgets(buffer, sizeof(buffer), file);

                        sscanf(buffer, "Score: %f", &newScore->score);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "Credit: %f", &newScore->credit);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "GPA: %f", &newScore->gpa);
                        insertAcademicScore(newStudent, newScore);
                    }
                }
                // ��ȡ��ѧ�����´�ҵ�ƻ���Ŀ����
                if (strncmp(buffer, PROJECT_START, strlen(PROJECT_START)) == 0) {
                    while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, PROJECT_END, strlen(PROJECT_END)) != 0) {
                        InnovationProject* newProject = (InnovationProject*)malloc(sizeof(InnovationProject));
                        if (newProject == NULL) {
                            handleInputError("�ڴ����ʧ��");
                            break;
                        }
                        memset(newProject, 0, sizeof(InnovationProject));
                        //fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "ProjectName: %s", newProject->projectName);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "GPA: %f", &newProject->gpa);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "LeaderOrSecondLeader: %d", &newProject->isLeaderOrSecondLeader);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "ProjectLevel: %s", newProject->projectLevel);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "IsFinished: %d", &newProject->isFinished);
                        insertInnovationProject(newStudent, newProject);
                    }
                }

                // ��ȡѧ����������
                if (strncmp(buffer, PAPER_START, strlen(PAPER_START)) == 0) {
                    while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, PAPER_END, strlen(PAPER_END)) != 0) {
                        AcademicPaper* newPaper = (AcademicPaper*)malloc(sizeof(AcademicPaper));
                        if (newPaper == NULL) {
                            handleInputError("�ڴ����ʧ��");
                            break;
                        }
                        memset(newPaper, 0, sizeof(AcademicPaper));
                        //fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "Title: %s", newPaper->title);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "JournalName: %s", newPaper->journalName);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "IsFirstAuthor: %d", &newPaper->isFirstAuthor);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "JournalLevel: %d", &newPaper->journalLevel);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "GPA: %f", &newPaper->gpa);
                        insertAcademicPaper(newStudent, newPaper);
                    }
                }

                // ��ȡ�������ѧ�ƾ�������
                if (strncmp(buffer, COMPETITION_START, strlen(COMPETITION_START)) == 0) {
                    while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, COMPETITION_END, strlen(COMPETITION_END)) != 0) {
                        Competition* newCompetition = (Competition*)malloc(sizeof(Competition));
                        if (newCompetition == NULL) {
                            handleInputError("�ڴ����ʧ��");
                            break;
                        }
                        memset(newCompetition, 0, sizeof(Competition));
                        //fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "CompetitionName: %s", newCompetition->competitionName);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "TeamSize: %d", &newCompetition->teamSize);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "CompetitionLevel: %s", newCompetition->competitionLevel);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "Rank: %d", &newCompetition->rank);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "CompetitionType: %c", &newCompetition->competitionType);
                        fgets(buffer, sizeof(buffer), file);
                        sscanf(buffer, "GPA: %f", &newCompetition->gpa);
                        insertCompetition(newStudent, newCompetition);
                    }
                }
                if (strncmp(buffer, STUDENT_END, strlen(STUDENT_END)) == 0) {
                    break;
                }
            }
        }
    }
    fclose(file); // �ر��ļ�

    // ���ع���Ա��Ϣ
    FILE* adminFile = fopen("admin.txt", "r");
    if (adminFile == NULL) {
        handleInputError("���ļ�ʧ��");
        return;
    }
    //freeAdminMemory(&adminsList); // ��չ���Ա����

    while (fgets(buffer, sizeof(buffer), adminFile)) {
        if (strncmp(buffer, ADMIN_START, strlen(ADMIN_START)) == 0) {
            AdminInfo* newAdmin = (AdminInfo*)malloc(sizeof(AdminInfo));
            if (newAdmin == NULL) {
                handleInputError("�ڴ����ʧ��");
                break;
            }
            memset(newAdmin, 0, sizeof(AdminInfo));
            fgets(buffer, sizeof(buffer), adminFile);
            sscanf(buffer, "Username: %s", newAdmin->username);
            fgets(buffer, sizeof(buffer), adminFile);
            sscanf(buffer, "Password: %s", newAdmin->password);
            newAdmin->next = adminsList;
            adminsList = newAdmin;
        }
    }
    fclose(adminFile); // �ر��ļ�
}

// ���㲢����ѧ�������ʼӷ�GPA
void calculateQualityGPA(StudentInfo *student) {
    // ���㲢����ѧ�������ʼӷ�GPA

    if (student == NULL) {
        return; // ��������ѧ��ָ��Ϊ�գ���ֱ�ӷ���
    }
    float totalQualityPoints = 0.00f; // �����ۼ����ʼӷ�GPAֵ
    float gpa1 = 0.00f; // ���ڴ�ѧ�����´�ҵ��Ŀ�ӷ�GPA
    float gpa2 = 0.00f; // ����ѧ�������������ֵ
    float gpa3 = 0.00f; // ���ڴ�ѧ�����´�ҵ�ƻ���Ŀ�������ֵ

    // ѧ���Ĵ�ѧ�����´�ҵ�ƻ���Ŀ����
    InnovationProject *currentProject = student->innovationProjects;
    if (currentProject != NULL) {
        gpa1 = currentProject->gpa; // �ۼ�ÿ����Ŀ��GPAֵ
    }

    // ѧ����ѧ����������
    AcademicPaper *currentPaper = student->academicPapers;
    while (currentPaper != NULL) {
        if(currentPaper->gpa > gpa2){
            gpa2 = currentPaper->gpa;
        }
        currentPaper = currentPaper->next; // �ƶ�����һ�����Ľڵ�
    }

    // ѧ���ļ������ѧ�ƾ�������
    Competition *currentCompetition = student->competitions;
    while (currentCompetition != NULL) {
        if(currentCompetition->gpa > gpa3){
            gpa3 = currentCompetition->gpa;
        }
        currentCompetition = currentCompetition->next; // �ƶ�����һ�������ڵ�
    }
    totalQualityPoints = gpa1 + gpa2 + gpa3;
    student->qualityGpa = totalQualityPoints; // �����ʼӷ�GPA
}

// �����ѧ�����´�ҵ�ƻ���Ŀ�ӷ�GPA
float calculateInnovationProjectGPA(InnovationProject *project) {
    // ������Ŀ������Ƿ�������ӷ�GPA
    float gpa = 0.00f;
    if (project == NULL) {
        return gpa; // ����������Ŀָ��Ϊ�գ���ֱ�ӷ���
    }
    if (project->isFinished == 1) {
        // �����Ŀ�ѽ��⣬������Ŀ�������ӷ�GPA
        if (strcmp(project->projectLevel, "���Ҽ�") == 0) {
            //��һ������
            if (project->isLeaderOrSecondLeader == 1) {
                gpa = 0.10f;
            } else if(project->isLeaderOrSecondLeader == 2) {
                gpa = 0.05f;
            }
        }
    }
    return gpa;
}

// ����ѧ�����ļӷ�GPA
float calculatePaperGPA(AcademicPaper *paper) {
    // �����ڿ�������Ƿ�Ϊ��һ���߼���ӷ�GPA
    float gpa = 0.00f;
    if (paper == NULL) {
        return gpa; // ������������ָ��Ϊ�գ���ֱ�ӷ���
    }
    if (paper->isFirstAuthor == 1) {
        // ����ǵ�һ���ߣ������ڿ��������ӷ�GPA
        switch (paper->journalLevel) {
            case 1:
                gpa = 0.40f;
                break;
            case 2:
                gpa = 0.20f;
                break;
            case 3:
                gpa = 0.10f;
                break;
            case 4:
                gpa = 0.05f;
                break;
            case 5:
                gpa = 0.02f;
                break;
            case 6:
                gpa = 0.01f;
                break;
            default:
                gpa = 0.00f;
        }
    }
    return gpa;
}

// ����������ѧ�ƾ����ӷ�GPA
float calculateCompetitionGPA(Competition *competition) {
    // ���ݻ񽱵ȼ��;����������ӷ�GPA
    float gpa = 0.00f;
    if (competition == NULL) {
        return gpa; // �������ľ���ָ��Ϊ�գ���ֱ�ӷ���
    }
    //����ǹ��Ҽ�
    if (strcmp(competition->competitionLevel, "���Ҽ�") == 0) {

        if (competition->competitionType=='A' && competition->rank == 1) {
            //A��һ�Ƚ�
            gpa = 0.40f;
        }else if (competition->competitionType=='A' && competition->rank == 2) {
            gpa = 0.20f;
        }else if (competition->competitionType=='A' && competition->rank == 3) {
            gpa = 0.10f;
        }else if (competition->competitionType=='B' && competition->rank == 1) {
            gpa = 0.20f;
        }else if (competition->competitionType=='B' && competition->rank == 2) {
            gpa = 0.10f;
        }else if (competition->competitionType=='B' && competition->rank == 3) {
            gpa = 0.05f;
        }else if (competition->competitionType=='C' && competition->rank == 1) {
            gpa = 0.10f;
        }else if (competition->competitionType=='C' && competition->rank == 2) {
            gpa = 0.05f;
        }else if (competition->competitionType=='C' && competition->rank == 3) {
            gpa = 0.02f;
        }else{
            gpa = 0.00f;
        }
    }
    return gpa;
}

// ��ʾѧ���Ĵ�ѧ�����´�ҵ�ƻ���Ŀ
void displayInnovationProjects(StudentInfo *student) {
    // ��ʾѧ���Ĵ�ѧ�����´�ҵ�ƻ���Ŀ
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    InnovationProject *current = student->innovationProjects;
    if (current == NULL) {
        printf("ѧ��û�д�ѧ�����´�ҵ�ƻ���Ŀ��\n");
        return;
    }else{
        printf("��ѧ�����´�ҵ�ƻ���Ŀ��\n");
        printf("��Ŀ����: %s, �Ƿ����: %d, ��Ŀ����: %s, �Ƿ�Ϊ��Ŀ������: %d, �ӷ�: %.2f\n", current->projectName, current->isFinished, current->projectLevel, current->isLeaderOrSecondLeader, current->gpa);
    }
}

// ��ʾѧ����ȫ��ѧ������
void displayAcademicPapers(StudentInfo *student) {
    // ��ʾѧ����ѧ������
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    AcademicPaper *current = student->academicPapers;
    if (current == NULL) {
        printf("ѧ��û��ѧ�����ġ�\n");
        return;
    }
    printf("ѧ�����ģ�\n");
    while (current != NULL) {
        printf("���ı���: %s, �ڿ�����: %s, �Ƿ��һ����: %d, �ڿ�����: %d, �ӷ�: %.2f\n", current->title, current->journalName, current->isFirstAuthor, current->journalLevel, current->gpa);
        current = current->next;
    }
    printf("����ѧ��������ȫ����ʾ��ɡ�\n");
}

// ��ʾѧ����ȫ���������ѧ�ƾ���
void displayCompetitions(StudentInfo *student) {
    // ��ʾѧ���ļ������ѧ�ƾ���
    if (student == NULL) {
        printf("ѧ����ϢΪ�ա�\n");
        return;
    }
    Competition *current = student->competitions;
    if (current == NULL) {
        printf("��ѧ��û�м������ѧ�ƾ�����\n");
        return;
    }
    printf("�������ѧ�ƾ�����\n");
    while (current != NULL) {
        printf("��������: %s, ������������: %d, ��������: %s, �񽱵ȼ�: %d, �������: %c, �ӷ�: %.2f\n", current->competitionName, current->teamSize, current->competitionLevel, current->rank, current->competitionType, current->gpa);
        current = current->next;
    }
    printf("���м������ѧ�ƾ�����ȫ����ʾ��ɡ�\n");
}

//�޸Ĵ�ѧ�����´�ҵ�ƻ���Ŀ
void modifyInnovationProject(StudentInfo *student) {
    // �޸Ĵ�ѧ�����´�ҵ�ƻ���Ŀ
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    if (student->innovationProjects == NULL) {
        printf("��ǰû����Ŀ�����޸ġ�\n");
    }
    int choice;
    do{
        printf("��ѡ���޸����ͣ�\n");
        printf("1. �޸���Ŀ����\n");
        printf("2. �޸���Ŀ����\n");
        printf("3. �޸��Ƿ����\n");
        printf("4. �޸��Ƿ�Ϊ��Ŀ������\n");
        printf("5. ������һ���˵�\n");
        scanf("%d", &choice);
        // �ж��û�ѡ���Ƿ�Ϸ�
        while (choice < 1 || choice > 5) {
            handleInputError("ѡ��Ӧ��1-5֮�䣬���������룺");
            scanf("%d", &choice);
            return;
        }
        switch (choice) {
            case 1: {
                // �޸���Ŀ����
                InnovationProject *current = student->innovationProjects;
                printf("������Ҫ�޸ĵ���Ŀ���ƣ�����q�˳�����");
                char projectName[MAX_NAME_LENGTH];
                scanf("%s", projectName);
                if (strcmp(projectName, "q") == 0) {
                    break;
                }
                strcpy(current->projectName, projectName);
                printf("��Ŀ�����޸ĳɹ���\n");
                break;
            }
            case 2: {
                // �޸���Ŀ����
                InnovationProject *current = student->innovationProjects;
                printf("������Ҫ�޸ĵļ�������q�˳�����");
                char projectLevel[MAX_NAME_LENGTH];
                scanf("%s", projectLevel);
                if (strcmp(projectLevel, "q") == 0) {
                    break;
                }
                //�ж������Ƿ�Ϸ�
                while(!isProjectLevelValid(projectLevel)){
                    handleInputError("����ӦΪ���Ҽ���ʡ����У�������������룺");
                    scanf("%s", projectLevel);
                }
                strcpy(current->projectLevel, projectLevel);
                printf("��Ŀ�����޸ĳɹ���\n");
                break;
            }
            case 3: {
                // �޸��Ƿ����
                InnovationProject *current = student->innovationProjects;
                printf("�������Ƿ���⣨1. �� 2. �񣩣�");
                int isFinished;
                scanf("%d", &isFinished);
                //�ж������Ƿ�Ϸ�
                while(isFinished != 1 && isFinished != 2){
                    handleInputError("����ӦΪ1��2�����������룺");
                    scanf("%d", &isFinished);
                }
                current->isFinished = isFinished;
                printf("�Ƿ�����޸ĳɹ���\n");
                break;
            }
            case 4: {
                // �޸��Ƿ�Ϊ��Ŀ������
                InnovationProject *current = student->innovationProjects;
                printf("�������Ƿ�Ϊ��Ŀ�����ˣ�0.���Ǹ����� 1. ��һ������ 2. �ڶ������ˣ���");
                int isLeaderOrSecondLeader;
                scanf("%d", &isLeaderOrSecondLeader);
                //�ж������Ƿ�Ϸ�
                while(isLeaderOrSecondLeader != 0 && isLeaderOrSecondLeader != 1 && isLeaderOrSecondLeader != 2){
                    handleInputError("����ӦΪ0��1��2�����������룺");
                    scanf("%d", &isLeaderOrSecondLeader);
                }
                current->isLeaderOrSecondLeader = isLeaderOrSecondLeader;
                printf("�Ƿ�Ϊ��Ŀ�������޸ĳɹ���\n");
                break;
            }
        }
    }while(choice != 5);
}

//�޸�ѧ������
void modifyAcademicPaper(StudentInfo *student) {
    // �޸�ѧ������
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    if (student->academicPapers == NULL) {
        printf("��ǰû��ѧ�����Ŀ����޸ġ�\n");
    }
    int choice;
    do{
        printf("��ѡ���޸����ͣ�\n");
        printf("1. �޸����ı���\n");
        printf("2. �޸��ڿ�����\n");
        printf("3. �޸��Ƿ�Ϊ��һ����\n");
        printf("4. �޸��ڿ�����\n");
        printf("5. ������һ���˵�\n");
        scanf("%d", &choice);
        // �ж��û�ѡ���Ƿ�Ϸ�
        while (choice < 1 || choice > 5) {
            handleInputError("ѡ��Ӧ��1-5֮�䣬���������룺");
            scanf("%d", &choice);
            return;
        }
        switch (choice) {
            case 1: {
                // �޸����ı���
                AcademicPaper *current = student->academicPapers;
                printf("������Ҫ�޸ĵ����ı��⣨����q�˳�����");
                char title[MAX_NAME_LENGTH];
                scanf("%s", title);
                if (strcmp(title, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->title, title) == 0) {
                        printf("�ҵ�����: %s, ��ǰ�ڿ�����: %s\n", current->title, current->journalName);
                        printf("�������µ����ı���: ");
                        scanf("%s", current->title);
                        printf("���ı����޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                break;
            }
            case 2: {
                // �޸��ڿ�����
                AcademicPaper *current = student->academicPapers;
                printf("������Ҫ�޸ĵ����ı��⣨����q�˳�����");
                char title[MAX_NAME_LENGTH];
                scanf("%s", title);
                if (strcmp(title, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->title, title) == 0) {
                        printf("�ҵ�����: %s, ��ǰ�ڿ�����: %s\n", current->title, current->journalName);
                        printf("�������µ��ڿ�����: ");
                        scanf("%s", current->journalName);
                        printf("�ڿ������޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                break;
            }
            case 3: {
                // �޸��Ƿ�Ϊ��һ����
                AcademicPaper *current = student->academicPapers;
                printf("������Ҫ�޸ĵ����ı��⣨����q�˳�����");
                char title[MAX_NAME_LENGTH];
                scanf("%s", title);
                if (strcmp(title, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->title, title) == 0) {
                        printf("�ҵ�����: %s, ��ǰ�Ƿ�Ϊ��һ����: %d\n", current->title, current->isFirstAuthor);
                        printf("�������Ƿ�Ϊ��һ���ߣ�0. �� 1. �ǣ�: ");
                        int isFirstAuthor;
                        scanf("%d", &isFirstAuthor);
                        //�ж������Ƿ�Ϸ�
                        while(isFirstAuthor != 0 && isFirstAuthor != 1){
                            handleInputError("����ӦΪ0��1�����������룺");
                            scanf("%d", &isFirstAuthor);
                        }
                        current->isFirstAuthor = isFirstAuthor;
                        printf("�Ƿ�Ϊ��һ�����޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                break;
            }
            case 4: {
                // �޸��ڿ�����
                AcademicPaper *current = student->academicPapers;
                printf("������Ҫ�޸ĵ����ı��⣨����q�˳�����");
                char title[MAX_NAME_LENGTH];
                scanf("%s", title);
                if (strcmp(title, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->title, title) == 0) {
                        printf("�ҵ�����: %s, ��ǰ�ڿ�����: %d\n", current->title, current->journalLevel);
                        current->journalLevel = 0;
                        calculateJournalLevel(current);
                        printf("�ڿ������޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                break;
            }
        }
    }while(choice != 5);
}

//�޸ļ������ѧ�ƾ���
void modifyCompetition(StudentInfo *student) {
    // �޸ļ������ѧ�ƾ���
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    if (student->competitions == NULL) {
        printf("��ǰû�о��������޸ġ�\n");
    }
    int choice;
    do{
        printf("��ѡ���޸����ͣ�\n");
        printf("1. �޸ľ�������\n");
        printf("2. �޸Ĳ�����������\n");
        printf("3. �޸ľ�������\n");
        printf("4. �޸Ļ񽱵ȼ�\n");
        printf("5. �޸ľ������\n");
        printf("6. ������һ���˵�\n");
        scanf("%d", &choice);
        // �ж��û�ѡ���Ƿ�Ϸ�
        while (choice < 1 || choice > 6) {
            handleInputError("����������������롣\n");
            scanf("%d", &choice);
        }
        switch (choice) {
            case 1: {
                // �޸ľ�������
                Competition *current = student->competitions;
                printf("������Ҫ�޸ĵľ������ƣ�����q�˳�����");
                char competitionName[MAX_NAME_LENGTH];
                scanf("%s", competitionName);
                if (strcmp(competitionName, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->competitionName, competitionName) == 0) {
                        printf("�ҵ�����: %s, ��ǰ������������: %d\n", current->competitionName, current->teamSize);
                        printf("�������µľ�������: ");
                        scanf("%s", current->competitionName);
                        printf("���������޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                if (current == NULL) {
                    printf("δ�ҵ�ָ���ľ�����\n");
                }
                break;
            }
            case 2: {
                // �޸Ĳ�����������
                Competition *current = student->competitions;
                printf("������Ҫ�޸ĵľ������ƣ�����q�˳�����");
                char competitionName[MAX_NAME_LENGTH];
                scanf("%s", competitionName);
                if (strcmp(competitionName, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->competitionName, competitionName) == 0) {
                        printf("�ҵ�����: %s, ��ǰ������������: %d\n", current->competitionName, current->teamSize);
                        printf("�������µĲ�����������: ");
                        scanf("%d", &current->teamSize);
                        printf("�������������޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                if (current == NULL) {
                    printf("δ�ҵ�ָ���ľ�����\n");
                }
                break;
            }
            case 3: {
                // �޸ľ�������
                Competition *current = student->competitions;
                printf("������Ҫ�޸ĵľ������ƣ�����q�˳�����");
                char competitionName[MAX_NAME_LENGTH];
                scanf("%s", competitionName);
                if (strcmp(competitionName, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->competitionName, competitionName) == 0) {
                        printf("�ҵ�����: %s, ��ǰ��������: %s\n", current->competitionName, current->competitionLevel);
                        printf("�������µľ�������: ");
                        scanf("%s", current->competitionLevel);
                        //�ж������Ƿ�Ϸ�
                        while(!isCompetitionLevelValid(current->competitionLevel)){
                            handleInputError("����ӦΪ���Ҽ���ʡ����У�������������룺");
                            scanf("%s", current->competitionLevel);
                        }
                        printf("���������޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                if (current == NULL) {
                    printf("δ�ҵ�ָ���ľ�����\n");
                }
                break;
            }
            case 4: {
                // �޸Ļ񽱵ȼ�
                Competition *current = student->competitions;
                printf("������Ҫ�޸ĵľ������ƣ�����q�˳�����");
                char competitionName[MAX_NAME_LENGTH];
                scanf("%s", competitionName);
                if (strcmp(competitionName, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->competitionName, competitionName) == 0) {
                        printf("�ҵ�����: %s, ��ǰ�񽱵ȼ�: %d\n", current->competitionName, current->rank);
                        printf("�������µĻ񽱵ȼ�: (1, 2, 3)");
                        int rank;
                        scanf("%d", &rank);
                        //�ж������Ƿ�Ϸ�
                        while(!isRankValid(rank)){
                            handleInputError("�ȼ�ӦΪ1��2��3�����������룺");
                            scanf("%d", &rank);
                        }
                        current->rank = rank;
                        printf("�񽱵ȼ��޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                if (current == NULL) {
                    printf("δ�ҵ�ָ���ľ�����\n");
                }
                break;
            }
            case 5: {
                // �޸ľ������
                Competition *current = student->competitions;
                printf("������Ҫ�޸ĵľ������ƣ�����q�˳�����");
                char competitionName[MAX_NAME_LENGTH];
                scanf("%s", competitionName);
                if (strcmp(competitionName, "q") == 0) {
                    break;
                }
                while (current != NULL) {
                    if (strcmp(current->competitionName, competitionName) == 0) {
                        printf("�ҵ�����: %s, ��ǰ�������: %c\n", current->competitionName, current->competitionType);
                        printf("�������µľ������: (A, B, C)");
                        char competitionType;
                        scanf("%c", &competitionType);
                        //�ж������Ƿ�Ϸ�
                        while(!isCompetitionTypeValid(competitionType)){
                            handleInputError("���ӦΪA��B��C�����������룺");
                            scanf("%c", &competitionType);
                        }
                        current->competitionType = competitionType;
                        printf("��������޸ĳɹ���\n");
                        break;
                    }
                    current = current->next;
                }
                if (current == NULL) {
                    printf("δ�ҵ�ָ���ľ�����\n");
                }
                break;
            }
        }
    } while (choice != 6);
}

//ɾ����ѧ�����´�ҵ��Ŀ
void deleteInnovationProject(StudentInfo *student) {
    // ɾ����ѧ�����´�ҵ�ƻ���Ŀ
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    if (student->innovationProjects == NULL) {
        printf("��ǰû����Ŀ����ɾ����\n");
    }
    InnovationProject *current = student->innovationProjects;
    printf("������Ҫɾ������Ŀ���ƣ�����q�˳�����");
    char projectName[MAX_NAME_LENGTH];
    scanf("%s", projectName);
    if (strcmp(projectName, "q") == 0) {
        return;
    }
    if (current != NULL && strcmp(current->projectName, projectName) == 0) {
        // Ҫɾ������ͷ�ڵ�
        student->innovationProjects = NULL;
        free(current); // �ͷű�ɾ���ڵ���ڴ�
        calculateQualityGPA(student);  //���¼������ʼӷ�GPA
        calculateAcademics(student);  //���¼���GPA
        printf("��Ŀɾ���ɹ���\n");
        return;
    }
    if (current == NULL) {
        printf("δ�ҵ�ָ������Ŀ��\n");
    }
}

//ɾ��ѧ������
void deleteAcademicPaper(StudentInfo *student) {
    // ɾ��ѧ������
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    if (student->academicPapers == NULL) {
        printf("��ǰû�����Ŀ���ɾ����\n");
    }
    AcademicPaper *current = student->academicPapers;
    AcademicPaper *prev = NULL;
    printf("������Ҫɾ�������ı��⣨����q�˳�����");
    char title[MAX_NAME_LENGTH];
    scanf("%s", title);
    if (strcmp(title, "q") == 0) {
        return;
    }
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            if (prev == NULL) {
                // Ҫɾ������ͷ�ڵ�
                student->academicPapers = current->next;
            } else {
                // Ҫɾ�������м��β�ڵ�
                prev->next = current->next;
            }
            AcademicPaper *temp = current;
            current = current->next;
            free(temp); // �ͷű�ɾ���ڵ���ڴ�
            calculateQualityGPA(student);  //���¼������ʼӷ�GPA
            calculateAcademics(student);  //���¼���GPA
            printf("����ɾ���ɹ���\n");
            break;
        }
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("δ�ҵ�ָ�������ġ�\n");
    }
}

//ɾ���������ѧ�ƾ���
void deleteCompetition(StudentInfo *student) {
    // ɾ���������ѧ�ƾ���
    if (student == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    if (student->competitions == NULL) {
        printf("��ǰû�о�������ɾ����\n");
    }
    Competition *current = student->competitions;
    Competition *prev = NULL;
    printf("������Ҫɾ���ľ������ƣ�����q�˳�����");
    char competitionName[MAX_NAME_LENGTH];
    scanf("%s", competitionName);
    if (strcmp(competitionName, "q") == 0) {
        return;
    }
    while (current != NULL) {
        if (strcmp(current->competitionName, competitionName) == 0) {
            if (prev == NULL) {
                // Ҫɾ������ͷ�ڵ�
                student->competitions = current->next;
            } else {
                // Ҫɾ�������м��β�ڵ�
                prev->next = current->next;
            }
            Competition *temp = current;
            current = current->next;
            free(temp); // �ͷű�ɾ���ڵ���ڴ�
            calculateQualityGPA(student);  //���¼������ʼӷ�GPA
            calculateAcademics(student);  //���¼���GPA
            printf("����ɾ���ɹ���\n");
            break;
        }
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("δ�ҵ�ָ���ľ�����\n");
    }
}

//���༶��ʾѧ����Ϣ
void displayByClass(StudentInfo *head) {
    // ���༶��ʾѧ����Ϣ
    if (head == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    int classNumber;
    int grade;
    printf("�������꼶��");
    scanf("%d", &grade);
    //�ж������Ƿ��ڷ�Χ��
    while (!isGradeValid(grade)) {
        handleInputError("�꼶������1-4֮��,����������\n");
        scanf("%d", &grade);
    }
    printf("������༶��");
    scanf("%d", &classNumber);
    //�ж������Ƿ��ڷ�Χ��
    while (!isClassValid(classNumber)) {
        handleInputError("�༶������Ϊ����0������,����������\n");
        scanf("%d", &classNumber);
    }
    StudentInfo *current = head;
    while (current != NULL) {
        if (current->classNumber == classNumber && current->grade == grade) {
            printf("ѧ��: %s, ����: %s, �༶: %d\nѧҵGPA: %f, �ѻ���ѧ��: %.2f, ������Ŀ�ӷ�GPA: %.2f, ��Ȩƽ����: %f, �ۺ�GPA: %.2f\n\n",
                   current->studentID, current->name, current->classNumber, current->gpa, current->totalCredit,
                   current->qualityGpa, current->averageScore, current->totalGPA);
            current = current->next;
        }
    }
}

//��ʾѧ���˵�
void displayStudentMenu(){
    // ��ʾѧ���˵�
    char choice[MAX_NAME_LENGTH];
    do{
        printf("1. ��ʾȫ��ѧ����Ϣ\n");
        printf("2. ��ʾһ���꼶��ѧ����Ϣ\n");
        printf("3. ��ʾһ���༶��ѧ����Ϣ\n");
        printf("4. ������һ���˵�\n");

        scanf("%s", choice);
        //�ж��û�ѡ��ִ����Ӧ����
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0) {
            handleInputError("ѡ��Ӧ��1-4֮�䣬���������룺");
            scanf("%s", choice);
        }
        if (strcmp(choice, "1") == 0) {
            displayAllStudents(studentsList);
        } else if (strcmp(choice, "2") == 0) {
            displayByGrade(studentsList);
        } else if (strcmp(choice, "3") == 0) {
            displayByClass(studentsList);
        } else if (strcmp(choice, "4") == 0) {
            return;
        }
    }while(strcmp(choice, "4") != 0);
}

//���꼶��ʾѧ����Ϣ
void displayByGrade(StudentInfo *head) {
    // ���꼶��ʾѧ����Ϣ
    if (head == NULL) {
        handleInputError("ѧ����Ϣ����Ϊ�ա�\n");
        return;
    }
    int grade;
    printf("�������꼶��");
    scanf("%d", &grade);
    //�ж������Ƿ��ڷ�Χ��
    while (!isGradeValid(grade)) {
        handleInputError("�꼶������1-4֮��,����������\n");
        scanf("%d", &grade);
    }
    StudentInfo *current = head;
    while (current != NULL) {
        if (current->grade == grade) {
            printf("ѧ��: %s, ����: %s, �༶: %d\nѧҵGPA: %f, �ѻ���ѧ��: %.2f, ������Ŀ�ӷ�GPA: %.2f, ��Ȩƽ����: %f, �ۺ�GPA: %.2f\n\n",
                   current->studentID, current->name, current->classNumber, current->gpa, current->totalCredit,
                   current->qualityGpa, current->averageScore, current->totalGPA);
            current = current->next;
        }
    }
}

//���꼶����
void sortByGrade(StudentInfo **head) {
    printf("������Ҫ������꼶\n");
    int grade;
    scanf("%d", &grade);
    //�ж������Ƿ��ڷ�Χ��
    while (!isGradeValid(grade)) {
        handleInputError("�꼶������1-4֮��,����������\n");
        scanf("%d", &grade);
    }
    char choice[MAX_NAME_LENGTH];

    // ����һ����ʱ�������ڴ�Ű��꼶ɸѡ��ѧ��
    StudentInfo *tempList = NULL;
    StudentInfo *current = *head;
    int length = 0;
    while (current != NULL) {
        if (current->grade == grade) {
            StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
            if (newStudent == NULL) {
                handleInputError("�ڴ����ʧ��");
                break;
            }
            //����ѧ����Ϣ
            memcpy(newStudent, current, sizeof(StudentInfo));
            newStudent->next = NULL;
            insertStudent(&tempList, newStudent);//����ѧ����Ϣ
            length++;
        }
        current = current->next;
    }
    do{
        printf("��ѡ������ʽ��\n");
        printf("1. ��ѧ������\n");
        printf("2. ��ѧҵGPA����\n");
        printf("3. ����Ȩƽ��������\n");
        printf("4. �����ܼ�������\n");
        printf("5. ����\n");


        scanf("%s", choice);
        //�ж������Ƿ��ڷ�Χ��
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0) {
            handleInputError("ѡ��Ӧ��1-5֮�䣬���������룺");
            scanf("%s", choice);
        }
        if(strcmp(choice, "1") == 0){
            sortList(&tempList, compareByStudentID,length);
        }else if(strcmp(choice, "2") == 0){
            sortList(&tempList, compareByGPA,length);
        }else if(strcmp(choice, "3") == 0){
            sortList(&tempList, compareByAverageScore,length);
        }else if(strcmp(choice, "4") == 0){
            sortList(&tempList, compareByTotalGPA,length);
        }else if(strcmp(choice, "5") == 0){
            return;
        }
        displayAllStudents(tempList);//��ʾ������ѧ����Ϣ
    }while(strcmp(choice, "5") != 0);

    freeTmpMemory(&tempList);//�ͷ���ʱ������ڴ�
}

//���༶����
void sortByClass(StudentInfo **head){
    printf("������Ҫ������꼶��1.��һ 2.��� 3.���� 4.���ģ�\n");
    int grade;
    scanf("%d", &grade);

    //�ж������Ƿ��ڷ�Χ��
    while(!isGradeValid(grade)){
        handleInputError("�꼶������1-4֮��,����������\n");
        scanf("%d", &grade);
    }

    printf("������Ҫ����İ༶��һ����������\n");
    int classNumber;
    scanf("%d", &classNumber);
    //�ж������Ƿ���������
    while (!isClassValid(classNumber)) {
        handleInputError("�༶������������,����������\n");
        scanf("%d", &classNumber);
    }
    char choice[MAX_NAME_LENGTH];

    // ����һ����ʱ�������ڴ�Ű��꼶ɸѡ��ѧ��
    StudentInfo *tempList = NULL;
    StudentInfo *current = *head;
    int length = 0;
    while (current != NULL) {
        if (current->classNumber == classNumber) {
            StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
            if (newStudent == NULL) {
                handleInputError("�ڴ����ʧ��");
                break;
            }
            memcpy(newStudent, current, sizeof(StudentInfo));//����ѧ����Ϣ
            newStudent->next = NULL;
            insertStudent(&tempList, newStudent);//����ѧ����Ϣ
            length++;
        }
        current = current->next;
    }
    do{
        printf("��ѡ������ʽ��\n");
        printf("1. ��ѧ������\n");
        printf("2. ��ѧҵGPA����\n");
        printf("3. ����Ȩƽ��������\n");
        printf("4. �����ܼ�������\n");
        printf("5. ����\n");



        scanf("%s", choice);
        //�ж������Ƿ��ڷ�Χ��
        while (strcmp(choice, "1") != 0 && strcmp(choice, "2") != 0 && strcmp(choice, "3") != 0 && strcmp(choice, "4") != 0 && strcmp(choice, "5") != 0) {
            handleInputError("ѡ��Ӧ��1-5֮�䣬���������룺");
            scanf("%s", choice);
        }
        if(strcmp(choice, "1") == 0){
            sortList(&tempList, compareByStudentID,length);
        }else if(strcmp(choice, "2") == 0){
            sortList(&tempList, compareByGPA,length);
        }else if(strcmp(choice, "3") == 0){
            sortList(&tempList, compareByAverageScore,length);
        }else if(strcmp(choice, "4") == 0){
            sortList(&tempList, compareByTotalGPA,length);
        }else if(strcmp(choice, "5") == 0){
            return;
        }
        displayAllStudents(tempList);//��ʾ������ѧ����Ϣ
    }while(strcmp(choice, "5") != 0);
    freeTmpMemory(&tempList);//�ͷ���ʱ������ڴ�
}

//�ж�ѧ���Ƿ�Ϸ�
int isStudentIDValid(const char *studentID) {
    // �ж�ѧ���Ƿ�Ϸ�
    if (strlen(studentID) != STUDENT_ID_LENGTH) {
        return 0;
    }
    for (int i = 0; i < STUDENT_ID_LENGTH; i++) {
        if (!isDigit(studentID[i])) {
            return 0;
        }
    }
    return 1;
}

//�ж��ַ��Ƿ�������
int isDigit(char c) {
    return c >= '0' && c <= '9';
}

//�ж��꼶�Ƿ�Ϸ�
int isGradeValid(int grade) {
    if (grade < 1 || grade > 4) {
        return 0;
    }
    return 1;
}

//�жϰ༶�Ƿ�Ϸ�
int isClassValid(int classNumber) {
    if(classNumber <= 0){
        return 0;
    }
    return 1;
}

// �жϷ����Ƿ�Ϸ�
int isScoreValid(float score){
    if (score < 0 || score > 100) {
        return 0;
    }
    return 1;
}

// �ж�ѧ���Ƿ�Ϸ�
int isCreditValid(float credit) {
    //ѧ��Ӧ����0С��MAX_CREDIT��Ϊ0.5��������
    // ���ѧ���Ƿ����0
    if (credit <= 0) {
        return 0; // ���Ϸ���ѧ��Ӧ����0
    }
    // ���ѧ���Ƿ�С�ڵ������ѧ��
    if (credit > MAX_CREDIT) {
        return 0; // ���Ϸ���ѧ�ֳ������ֵ
    }
    // ���ѧ���Ƿ�Ϊ0.5��������
    unsigned long int tmp;
    tmp = credit * 10;
    if (!(tmp % 5) && ((credit * 10 - tmp) < 0.000001)) {
        return 1; // �Ϸ���ѧ��Ϊ0.5��������
    }else{
        return 0; // ���Ϸ���ѧ�ֲ���0.5��������
    }
    // ����������������㣬��ѧ�ֺϷ�
    return 1;
}

// �ж������Ƿ�Ϸ�
int isPasswordValid(const char *password) {
    if (strlen(password) < 1 || strlen(password) > MAX_PASSWORD_LENGTH) {
        return 0;
    }
    return 1;
}

//�жϴ���Ŀ�����Ƿ�Ϸ�
int isProjectLevelValid(const char *projectLevel){
    if(strcmp(projectLevel, "���Ҽ�") != 0 && strcmp(projectLevel, "ʡ��") != 0 && strcmp(projectLevel, "У��") != 0){
        return 0;
    }
    return 1;
}

//�жϾ��������Ƿ�Ϸ�
int isCompetitionLevelValid(const char *competitionLevel){
    if(strcmp(competitionLevel, "���Ҽ�") != 0 && strcmp(competitionLevel, "ʡ��") != 0 && strcmp(competitionLevel, "У��") != 0){
        return 0;
    }
    return 1;
}

//�жϾ�������Ƿ�Ϸ�
int isCompetitionTypeValid(char competitionType){
    if(competitionType != 'A' && competitionType != 'B' && competitionType != 'C'){
        return 0;
    }
    return 1;
}

//�жϻ񽱵ȼ��Ƿ�Ϸ�
int isRankValid(int rank){
    if(rank != 1 && rank != 2 && rank != 3){
        return 0;
    }
    return 1;
}

//�ж��Ƿ�Ϊ�������Ƿ�Ϸ�
int isLeaderOrSecondLeaderValid(int isLeaderOrSecondLeader){
    if(isLeaderOrSecondLeader != 0 && isLeaderOrSecondLeader != 1 && isLeaderOrSecondLeader != 2){
        return 0;
    }
    return 1;
}

//�������ļ���
void calculateJournalLevel(AcademicPaper *academicPaper){
    // �������ļ���
    if(strcmp(academicPaper->journalName, "ҵ�繫��ѧ���ۺ϶����ڿ�") == 0){
        academicPaper->journalLevel = 1;
    }else if(strcmp(academicPaper->journalName, "CCF-A�ڿ�") == 0
             || strcmp(academicPaper->journalName, "CCF-A���鳤��") == 0
             || strcmp(academicPaper->journalName, "CCF-B������������/���ѧ������") == 0){
        academicPaper->journalLevel = 2;
    } else if(strcmp(academicPaper->journalName, "CCF-B�ڿ�") == 0
              || strcmp(academicPaper->journalName, "�п�Ժ�ڿ�����һ������") == 0
              || strcmp(academicPaper->journalName, "CCF-C������������/���ѧ������") == 0){
        academicPaper->journalLevel = 3;
    } else if(strcmp(academicPaper->journalName, "CCF-C�ڿ�����") == 0
              || strcmp(academicPaper->journalName, "CCF-B���鳤��") == 0
              || strcmp(academicPaper->journalName, "�п�Ժ�ڿ�������������") == 0
              || strcmp(academicPaper->journalName, "�����ѧ�������ѧ�������ѧ������") == 0){
        academicPaper->journalLevel = 4;
    } else if(strcmp(academicPaper->journalName, "Ӱ�����ӷ�0��SCI�����ڿ�����") == 0
              || strcmp(academicPaper->journalName, "CCF-C���鳤��") == 0
              || strcmp(academicPaper->journalName, "��Ϣ��ѧ") == 0
              || strcmp(academicPaper->journalName, "������о��뷢չ") == 0
              || strcmp(academicPaper->journalName, "����ѧ�����İ�") == 0
              || strcmp(academicPaper->journalName, "�Զ���ѧ�������ѧ������") == 0){
        academicPaper->journalLevel = 5;
    } else if(strcmp(academicPaper->journalName, "EI�����ڿ�") == 0){
        academicPaper->journalLevel = 6;
    }else{
        academicPaper->journalLevel = 0;
    }
}

// �ͷ�ѧ���ڴ�
void freeTmpMemory(StudentInfo **head) {
    // ���ѧ���б�ͷָ���Ƿ�Ϊ��
    if (*head == NULL) {
        return; // ���ͷָ��Ϊ�գ���û���ڴ���Ҫ�ͷ�
    }
    // �������ͷ�ÿ��ѧ���ڵ��ѧҵ�ɼ�����
    StudentInfo *currentStudent = *head;
    while (currentStudent != NULL) {
        // �ͷŵ�ǰѧ���ڵ���ڴ�
        StudentInfo *tempStudent = currentStudent;
        currentStudent = currentStudent->next;
        free(tempStudent); // �ͷŽڵ��ڴ�
    }
    // ����ѧ���б�ͷָ��ΪNULL
    *head = NULL;
}