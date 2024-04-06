#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义最大字符串长度
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 15
#define STUDENT_ID_LENGTH 8
#define MAX_CREDIT 8
// 特定字符串用于读取标记
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

// 学业成绩结构体
typedef struct AcademicScoreNode {
    char courseName[MAX_NAME_LENGTH];
    float score;    // 分数
    float credit; // 学分
    float gpa; // 单门课程绩点
    struct AcademicScoreNode *next;
} AcademicScoreNode;

// 大学生创新创业计划项目结构体
typedef struct InnovationProject {
    char projectName[MAX_NAME_LENGTH]; // 项目名称
    float gpa; // 加分值
    int isLeaderOrSecondLeader;// 是否为项目负责人,是否为第二负责人，国家级优秀结题项目负责人加0.1 GPA，第二名加0.05 GPA
    char projectLevel[5];// 项目级别，国家级，省级，校级
    int isFinished;    // 是否结题
} InnovationProject;

// 学术论文结构体
typedef struct AcademicPaper {
    char title[MAX_NAME_LENGTH]; // 论文标题
    char journalName[MAX_NAME_LENGTH]; // 期刊名称
    int isFirstAuthor; // 是否为第一作者

    //期刊级别，顶级加0.4，一级期刊加0.2 GPA，二级期刊加0.1 GPA，三级期刊加0.05 GPA，四级期刊加0.02 GPA，五级期刊加0.01 GPA
    int journalLevel;

    float gpa; // 加分值
    struct AcademicPaper *next;
} AcademicPaper;

// 计算机类学科竞赛结构体
typedef struct Competition {
    char competitionName[MAX_NAME_LENGTH]; // 竞赛名称
    int teamSize;   //参赛队伍人数
    char competitionLevel[5]; // 竞赛级别,国家级，省级，校级
    int rank;   // 获奖等级，1表示一等奖，2表示二等奖，3表示三等奖
    char competitionType;   //竞赛类别，A类，B类，C类
    float gpa; // 加分值
    struct Competition *next;
} Competition;

// 学生信息结构体
typedef struct StudentInfo {
    char studentID[STUDENT_ID_LENGTH];// 学号
    char name[MAX_NAME_LENGTH];// 姓名
    char password[MAX_PASSWORD_LENGTH];// 密码
    int classNumber;// 班级
    int grade;      //年级（大一、大二、大三、大四）
    float gpa;  // 学业GPA
    float totalCredit;// 总学分
    float qualityGpa;// 素质加分GPA
    float averageScore;// 加权平均分
    float totalGPA;// 总绩点
    AcademicScoreNode *academicScores;// 学业成绩
    InnovationProject *innovationProjects;// 大学生创新创业计划项目
    AcademicPaper *academicPapers;// 学术论文
    Competition *competitions;// 计算机类学科竞赛
    struct StudentInfo *next;// 指向下一个学生的指针
} StudentInfo;

// 管理员信息结构体
typedef struct AdminInfo {
    char username[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    struct AdminInfo *next;
} AdminInfo;

// 全局变量
StudentInfo *studentsList = NULL;
AdminInfo *adminsList=NULL;
int count = 0;//记录密码错误次数

// 函数声明
void loginSystem();//登录系统
void studentLogin();//学生登录
void adminLogin();//管理员登录
void addStudentAccount();//添加学生账号
void addCourse(StudentInfo *student);//添加课程
void addQualityProject(StudentInfo *student);//添加素质加分项目
void modifyScoreMenu();//修改成绩、删除学生页面
void modifyAcademicScore(StudentInfo *student);//修改学业成绩
void modifyQualityScore(StudentInfo *student);//修改素质加分
void sortStudentsMenu();//排序学生页面
void saveToFile();//保存到文件
void loadFromFile();//从文件加载
void displayStudentInfo(StudentInfo *student);//显示学生信息概况
void displayAcademicScore(StudentInfo *student);//显示该学生全部学业成绩
void displayQualityScore(StudentInfo *student);//显示该学生全部素质加分
void displayAdminMenu(AdminInfo *admin);//显示管理员菜单
void displaySelfMenu(StudentInfo *student);//学生自己显示菜单
void freeMemory(StudentInfo **head);//释放内存
void freeAdminMemory(AdminInfo **head);//释放管理员内存
StudentInfo *createStudent();//创建新学生节点
StudentInfo *findStudentByID(const char *studentID);//查找学生
AdminInfo *findAdminByUsername(const char *username);//查找管理员
AcademicScoreNode *createAcademicScore();//创建新学业成绩节点
InnovationProject *createInnovationProject();//创建新大学生创新创业计划项目节点
AcademicPaper *createAcademicPaper();//创建新学术论文节点
Competition *createCompetition();//创建新计算机类学科竞赛节点
void insertAcademicScore(StudentInfo *student, AcademicScoreNode *scoreNode);//插入学业成绩链表
void insertInnovationProject(StudentInfo *student, InnovationProject *project);//插入大学生创新创业计划项目链表尾
void insertAcademicPaper(StudentInfo *student, AcademicPaper *paper);//插入学术论文链表尾
void insertCompetition(StudentInfo *student, Competition *competition);//插入计算机类学科竞赛链表尾
void insertStudent(StudentInfo **head, StudentInfo *newStudent);//插入学生链表头
void calculateGPA(StudentInfo *student);//计算GPA、总学分、加权平均分、总绩点
float calculateSingleGPA(const float *score);//计算单门课程绩点
void deleteStudentRecord(StudentInfo **head, char *studentID);//删除学生记录
void handleInputError(const char *message);//处理输入错误
void displayAllStudents(StudentInfo *head);//显示所有学生信息概况
void displayByClass(StudentInfo *head);//按班级显示学生信息
void displayByGrade(StudentInfo *head);//按年级显示学生信息
void sortList(StudentInfo **head, int (*compare)(const StudentInfo *, const StudentInfo *));//排序学生链表
int compareByStudentID(const StudentInfo *a, const StudentInfo *b);//按学号排序
int compareByGPA(const StudentInfo *a, const StudentInfo *b);//按学业GPA排序
int compareByAverageScore(const StudentInfo *a, const StudentInfo *b);//按加权平均分排序
int compareByTotalGPA(const StudentInfo *a, const StudentInfo *b);//按总绩点排序
void addAdminAccount();//添加管理员账号
float calculatePaperGPA(AcademicPaper *paper);//计算学术论文加分
float calculateCompetitionGPA(Competition *competition);//计算计算机类学科竞赛加分
float calculateInnovationProjectGPA(InnovationProject *project);//计算大学生创新创业计划项目加分
void calculateQualityGPA(StudentInfo *student);//计算素质加分GPA
void displayInnovationProjects(StudentInfo *student);//显示大学生创新创业计划项目
void displayAcademicPapers(StudentInfo *student);//显示学术论文
void displayCompetitions(StudentInfo *student);//显示计算机类学科竞赛
void modifyInnovationProject(StudentInfo *student);//修改大学生创新创业计划项目
void modifyAcademicPaper(StudentInfo *student);//修改学术论文
void modifyCompetition(StudentInfo *student);//修改计算机类学科竞赛
void deleteInnovationProject(StudentInfo *student);//删除大学生创新创业计划项目
void deleteAcademicPaper(StudentInfo *student);//删除学术论文
void deleteCompetition(StudentInfo *student);//删除计算机类学科竞赛
void displayStudentMenu();//显示学生的菜单
void sortByGrade(StudentInfo **head);//按年级排序
void sortByClass(StudentInfo **head);//按班级排序
int isStudentIDValid(const char *studentID);//判断学号是否合法
int isGradeValid(int grade);//判断年级是否合法
int isClassValid(int classNumber);//判断班级是否合法
int isScoreValid(float score);//判断分数是否合法
int isPasswordValid(const char *password);//判断密码是否合法
int isDigit(char c);//判断是否为数字
int isCreditValid(float credit);//判断学分是否合法
int isJournalLevelValid(int journalLevel);//判断期刊级别是否合法
int isCompetitionLevelValid(const char *competitionLevel);//判断竞赛级别是否合法
int isCompetitionTypeValid(char competitionType);//判断竞赛类别是否合法
int isProjectLevelValid(const char *projectLevel);//判断项目级别是否合法
int isRankValid(int rank);//判断获奖等级是否合法
int isLeaderOrSecondLeaderValid(int isLeaderOrSecondLeader);//判断是否为负责人是否合法

// 主函数
int main() {
    // 初始化管理员信息,用户名和密码均为admin
    AdminInfo *admin = (AdminInfo *)malloc(sizeof(AdminInfo));
    if (admin == NULL) {
        // 如果内存分配失败，打印错误信息并退出程序
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    strcpy(admin->username, "admin");
    strcpy(admin->password, "admin");
    admin->next = NULL;
    adminsList = admin;
    loadFromFile();
    // 进入系统
    loginSystem();
    saveToFile();
    return 0;
}

// 登录系统
void loginSystem() {
    int choice;
    printf("请选择登录类型：\n1. 学生\n2. 管理员\n");
    scanf("%d", &choice);
    while (choice != 1 && choice != 2) {
        printf("无效的选择，请重新输入。\n");
        scanf("%d", &choice);
    }
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
            printf("无效的选择。\n");
            exit(1);
        }
    }
}

// 学生登录
void studentLogin() {
    char studentID[STUDENT_ID_LENGTH], password[MAX_PASSWORD_LENGTH];
    printf("请输入学号和密码：\n");
    scanf("%s", studentID);//输入学号
    //判断学号是否存在
    while (findStudentByID(studentID) == NULL) {
        handleInputError("学号不存在，请重新输入。\n");
        scanf("%s", studentID);
    }
    scanf("%s", password);//输入密码
    //判断密码是否合法
    while (!isPasswordValid(password)) {
        handleInputError("密码应为1-15位数字或字母，请重新输入。\n");
        scanf("%s", password);
    }
    // 验证学生信息
    StudentInfo *student = findStudentByID(studentID);
    if (student && strcmp(student->password, password) == 0) {
        // 登录成功
        printf("学生登录成功。\n");
        // 显示学生菜单
        displaySelfMenu(student);
    } else {
        // 登录失败
        printf("学号或密码错误。\n");
        count++;
        if (count == 3) {
            printf("密码错误次数过多，退出系统。\n");
            exit(1);
        }else{
            studentLogin();
        }
    }
}

// 管理员登录
void adminLogin() {
    char username[MAX_NAME_LENGTH];
    printf("请输入管理员用户名：\n");
    scanf("%s", username);
    // 判断用户名是否存在
    while (findAdminByUsername(username) == NULL) {
        handleInputError("用户名不存在，请重新输入。\n");
        scanf("%s", username);
    }
    char password[MAX_PASSWORD_LENGTH];
    printf("请输入管理员密码：\n");
    scanf("%s", password);
    //判断密码是否合法
    while (!isPasswordValid(password)) {
        handleInputError("密码应为1-15位数字或字母，请重新输入。\n");
        scanf("%s", password);
    }
    AdminInfo *adminByUsername = findAdminByUsername(username);
    // 验证管理员密码
    if (adminByUsername && strcmp(adminByUsername->password, password) == 0) {
        // 登录成功
        printf("管理员登录成功。\n");
        // 显示管理员菜单
        displayAdminMenu(adminByUsername);
    } else {
        // 登录失败
        printf("密码错误。\n");
        count++;
        if (count == 3) {
            printf("密码错误次数过多，退出系统。\n");
            exit(1);
        }else{
            adminLogin();
        }
    }
}

// 添加学生账号
void addStudentAccount() {
    StudentInfo *newStudent = createStudent();
    // 计算GPA
    calculateGPA(newStudent);
    // 插入学生列表
    insertStudent(&studentsList, newStudent);
    printf("学生账号创建成功。\n");
}

// 添加课程
void addCourse(StudentInfo *student) {
    AcademicScoreNode *newCourse = createAcademicScore();
    printf("请输入课程信息：\n");
    printf("课程名称: ");
    scanf("%s", newCourse->courseName);
    printf("分数: ");
    scanf("%f", &newCourse->score);
    //判断分数是否合法
    while (!isScoreValid(newCourse->score)) {
        handleInputError("分数应为0-100之间的数，请重新输入。\n");
        scanf("%f", &newCourse->score);
    }
    printf("学分: ");
    scanf("%f", &newCourse->credit);
    //判断学分是否合法
    while (isCreditValid(newCourse->credit)){
        printf("学分不会超过8，请重新输入。\n");
        scanf("%f", &newCourse->credit);
    }
    newCourse->gpa = calculateSingleGPA(&newCourse->score);
    // 将新课程添加到指定学生的课程链表
    insertAcademicScore(findStudentByID(student->studentID), newCourse);
    printf("课程添加成功。\n");
}

// 添加素质加分项目
void addQualityProject(StudentInfo *student) {
    int choice;
    printf("请选择加分项目类型：\n");
    printf("1. 大学生创新创业计划项目\n");
    printf("2. 学术论文\n");
    printf("3. 计算机类学科竞赛\n");
    printf("4. 返回\n");
    scanf("%d", &choice);
    //判断选择是否合法
    while (choice != 1 && choice != 2 && choice != 3 && choice != 4) {
        printf("无效的选项，请重新输入。\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:{
            InnovationProject *newProject = createInnovationProject();
            insertInnovationProject(student, newProject);
            addQualityProject(student);
            break;
        }
        case 2:{
            AcademicPaper *newPaper = createAcademicPaper();
            insertAcademicPaper(student, newPaper);
            addQualityProject(student);
            break;
        }
        case 3:{
            Competition *newCompetition = createCompetition();
            insertCompetition(student, newCompetition);
            addQualityProject(student);
            break;
        }
        case 4:{
            printf("返回。\n");
            break;
        }
        default: {
            printf("无效的选项。\n");
        }
    }
}

// 编辑学生成绩或删除学生
void modifyScoreMenu() {
    char studentID[STUDENT_ID_LENGTH];
    printf("请输入学生学号: ");
    scanf("%s", studentID);
    //判断学号是否合法
    while (!isStudentIDValid(studentID)) {
        handleInputError("学号应为8位数字，请重新输入。\n");
        scanf("%s", studentID);
    }
    StudentInfo *student = findStudentByID(studentID);
    if (student != NULL) {
        int choice;
        printf("请选择修改类型：\n");
        printf("1. 修改学业成绩\n");
        printf("2. 添加学业成绩\n");
        printf("3. 修改素质加分\n");
        printf("4. 添加素质加分\n");
        printf("5. 修改年级\n");
        printf("6. 修改班级\n");
        printf("7. 删除学生\n");
        printf("8. 返回\n");
        scanf("%d", &choice);
        //判断选择是否合法
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7 && choice != 8) {
            handleInputError("选项应为1-8之间的数，请重新输入。\n");
            scanf("%d", &choice);
        }
        switch (choice) {
            case 1:{
                // 编辑学业成绩
                modifyAcademicScore(student);
                break;
            }
            case 2:{
                // 添加学业成绩
                addCourse(student);
                break;
            }
            case 3:{
                // 编辑素质加分
                modifyQualityScore(student);
                break;
            }
            case 4:{
                // 添加素质加分
                addQualityProject(student);
                break;
            }
            case 5:{
                // 修改年级
                printf("请输入新年级：（1.大一 2.大二 3.大三 4.大四）");
                scanf("%d", &student->grade);
                //判断年级是否合法
                while (!isGradeValid(student->grade)) {
                    handleInputError("年级应为1-4之间的数(1.大一 2.大二 3.大三 4.大四)，请重新输入。\n");
                    scanf("%d", &student->grade);
                }
                break;
            }
            case 6:{
                // 修改班级
                printf("请输入新班级：");
                scanf("%d", &student->classNumber);
                //判断班级是否合法
                while (!isClassValid(student->classNumber)) {
                    handleInputError("班级应为大于0的数，请重新输入。\n");
                    scanf("%d", &student->classNumber);
                }
                break;
            }
            case 7:{
                // 删除学生
                deleteStudentRecord(&studentsList, studentID);
                break;
            }
            case 8:{
                printf("返回。\n");
                // 返回上一级菜单
                break;
            }
            default: {
                printf("无效的选项。\n");
            }
        }
        // 更新GPA、总学分、加权平均分、总绩点
        calculateGPA(student);
        printf("成绩修改成功。\n");
    } else {
        printf("未找到学生信息。\n");
    }
}

// 排序学生
void sortStudentsMenu() {
    // 根据GPA或其他标准对学生列表进行排序
    int choice;
    printf("请选择排序范围：\n");
    printf("1. 按年级范围排序\n");
    printf("2. 按班级范围排序\n");
    printf("3. 返回\n");
    scanf("%d", &choice);
    //判断选择是否合法
    while (choice != 1 && choice != 2 && choice != 3) {
        handleInputError("选项应为1-3之间的数，请重新输入。\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:{
            sortByGrade(&studentsList);
            break;
        }
        case 2:{
            sortByClass(&studentsList);
            break;
        }
        case 3:{
            printf("返回。\n");
            break;
        }
        default: {
            printf("无效的选项。\n");
        }
    }
}

// 显示指定学生信息概况
void displayStudentInfo(StudentInfo *student) {
    // 显示学生的成绩和加分信息
    if (student == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    printf("学号: %s\n", student->studentID);
    printf("姓名: %s\n", student->name);
    printf("班级: %d\n", student->classNumber);
    printf("年级: %d\n", student->grade);
    printf("已获总学分: %.2f\n", student->totalCredit);
    printf("学业GPA: %f\n", student->gpa);
    printf("素质加分GPA: %.2f\n", student->qualityGpa);
    printf("加权平均分: %.2f\n", student->averageScore);
    printf("总绩点: %.2f\n", student->totalGPA);
    printf("学生信息显示完成。\n");
    int choice;
    printf("是否要继续显示详细信息：\n");
    printf("1. 显示所有学业成绩\n");
    printf("2. 显示所有素质加分\n");
    printf("3. 返回\n");
    scanf("%d", &choice);
    //判断选择是否合法
    while (choice != 1 && choice != 2 && choice != 3) {
        handleInputError("选项应为1-3之间的数，请重新输入。\n");
        scanf("%d", &choice);
    }
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
            printf("返回。\n");
            break;
        }
        default: {
            printf("无效的选项。\n");
        }
    }
}

// 显示管理员菜单
void displayAdminMenu(AdminInfo *admin) {
    int choice;
    do {
        printf("管理员菜单：\n");
        printf("1. 添加新学生\n");
        printf("2. 添加管理员账号\n");
        printf("3. 编辑学生成绩或删除学生\n");
        printf("4. 按不同要求排序学生\n");
        printf("5. 按不同范围显示学生信息\n");
        printf("6. 查找学生\n");
        printf("7. 保存到文件\n");
        printf("8. 从文件加载\n");
        printf("9. 更改密码\n");
        printf("10. 退出\n");
        printf("请选择操作：");
        scanf("%d", &choice);
        //判断选择是否合法
        while (choice < 1 || choice > 10) {
            handleInputError("选项应为1-10之间的数，请重新输入。\n");
            scanf("%d", &choice);
        }
        switch (choice) {
            case 1:{
                // 添加学生账号
                addStudentAccount();
                break;
            }
            case 2:{
                // 添加管理员账号
                addAdminAccount();
                break;
            }
            case 3:{
                // 编辑学生成绩或删除学生
                modifyScoreMenu();
                break;
            }
            case 4:{
                // 排序学生
                sortStudentsMenu();
                break;
            }
            case 5:{
                // 显示学生信息
                displayStudentMenu();
                break;
            }
            case 6:{
                // 查找
                printf("请输入需要查找学生的学号\n");
                char studentID[STUDENT_ID_LENGTH];
                scanf("%s", studentID);
                //判断学号是否合法
                while (!isStudentIDValid(studentID)) {
                    handleInputError("学号应为8位数字，请重新输入。\n");
                    scanf("%s", studentID);
                }
                StudentInfo *student = findStudentByID(studentID);
                if (student != NULL) {
                    displayStudentInfo(student);
                } else {
                    printf("未找到学生信息。\n");
                }
                break;
            }
            case 7:{
                // 保存到文件
                saveToFile();
                break;
            }
            case 8:{
                // 从文件加载
                loadFromFile();
                break;
            }
            case 9:{
                // 更改密码
                char newPassword[MAX_PASSWORD_LENGTH];
                printf("请输入新密码：（不超过15位）");
                scanf("%s", newPassword);
                //判断密码长度是否合法
                while (isPasswordValid(newPassword)) {
                    handleInputError("密码长度应为1-15位，请重新输入。\n");
                    scanf("%s", newPassword);
                }
                strcpy(admin->password, newPassword);
                printf("密码修改成功。\n");
                break;
            }
            case 10:{
                printf("管理员退出登录。\n");
                freeMemory(&studentsList);//释放内存
                freeAdminMemory(&adminsList);//释放内存
                break;
            }
            default: {
                printf("无效的选项，请重新输入。\n");
            }
        }
    } while (choice != 10);
}

//显示学生菜单
void displaySelfMenu(StudentInfo *student) {
    int choice;
    do {
        printf("学生菜单：\n");
        printf("1. 查看成绩\n");
        printf("2. 查看加分\n");
        printf("3. 修改密码\n");
        printf("4. 退出\n");
        printf("请选择操作：");
        scanf("%d", &choice);
        //判断选择是否合法
        while (choice < 1 || choice > 4) {
            handleInputError("选项应为1-4之间的数，请重新输入。\n");
            scanf("%d", &choice);
        }
        switch (choice) {
            case 1:{
                // 查看成绩
                displayAcademicScore(student);
                break;
            }
            case 2:{
                // 查看加分
                displayQualityScore(student);
                break;
            }
            case 3:{
                // 修改密码
                char newPassword[MAX_PASSWORD_LENGTH];
                printf("请输入新密码：");
                scanf("%s", newPassword);
                //判断密码长度是否合法
                while (isPasswordValid(newPassword)) {
                    handleInputError("密码长度应为1-15位，请重新输入。\n");
                    scanf("%s", newPassword);
                }
                strcpy(student->password, newPassword);
                printf("密码修改成功。\n");
                break;
            }
            case 4:{
                printf("学生退出登录。\n");
                freeMemory(&studentsList);//释放内存
                freeAdminMemory(&adminsList);//释放内存
                break;
            }
            default: {
                printf("无效的选项，请重新输入。\n");
            }
        }
    } while (choice != 4);
}

// 释放学生内存
void freeMemory(StudentInfo **head) {
    // 检查学生列表头指针是否为空
    if (*head == NULL) {
        return; // 如果头指针为空，则没有内存需要释放
    }

    // 遍历并释放每个学生节点的学业成绩链表
    StudentInfo *currentStudent = *head;
    while (currentStudent != NULL) {
        // 释放学业成绩链表中的每个节点
        AcademicScoreNode *currentScore = currentStudent->academicScores;
        while (currentScore != NULL) {
            AcademicScoreNode *temp = currentScore;
            currentScore = currentScore->next;
            free(temp); // 释放节点内存
        }
        // 释放大学生创新创业计划项目链表中的每个节点
        InnovationProject *currentProject = currentStudent->innovationProjects;
        if (currentProject != NULL) {
            InnovationProject *temp = currentProject;
            free(temp); // 释放节点内存
        }

        // 释放学术论文链表中的每个节点
        AcademicPaper *currentPaper = currentStudent->academicPapers;
        while (currentPaper != NULL) {
            AcademicPaper *temp = currentPaper;
            currentPaper = currentPaper->next;
            free(temp); // 释放节点内存
        }

        // 释放计算机类学科竞赛链表中的每个节点
        Competition *currentCompetition = currentStudent->competitions;
        while (currentCompetition != NULL) {
            Competition *temp = currentCompetition;
            currentCompetition = currentCompetition->next;
            free(temp); // 释放节点内存
        }

        // 释放当前学生节点的内存
        StudentInfo *tempStudent = currentStudent;
        currentStudent = currentStudent->next;
        free(tempStudent); // 释放节点内存
    }

    // 重置学生列表头指针为NULL
    *head = NULL;
}

// 插入学生记录
void insertStudent(StudentInfo **head, StudentInfo *newStudent) {
    // 将新学生插入到学生列表的头部
    if (head == NULL || newStudent == NULL) {
        return; // 如果头指针或新学生节点为空，则不执行操作
    }

    // 将新学生节点的next指针指向当前的头节点
    newStudent->next = *head;

    // 更新头指针为新学生节点
    *head = newStudent;
}

// 插入学业成绩
void insertAcademicScore(StudentInfo *student, AcademicScoreNode *scoreNode) {
    // 将新课程添加到学生的学业成绩链表
    if (student == NULL || scoreNode == NULL) {
        return; // 如果学生或成绩节点为空，则不执行操作
    }

    AcademicScoreNode *current = student->academicScores;
    if (current == NULL) {
        // 如果学生的成绩链表为空，则新节点成为第一个节点
        student->academicScores = scoreNode;
    } else {
        // 遍历链表直到找到最后一个节点
        while (current->next != NULL) {
            current = current->next;
        }
        // 将新节点添加到链表的末尾
        current->next = scoreNode;
    }
}

// 插入大学生创新创业计划项目
void insertInnovationProject(StudentInfo *student, InnovationProject *project) {
    // 将新项目添加到学生的大学生创新创业计划项目链表
    if (student == NULL ) {
        return; // 如果学生或项目节点为空，则不执行操作
    }

    InnovationProject *current = student->innovationProjects;
    if (current == NULL) {
        // 如果学生的项目链表为空，则新节点成为第一个节点
        student->innovationProjects = project;
        printf("大学生创新创业项目录入成功");
    } else {
        printf("已经存在大学生创新创业计划项目");
    }
}

// 插入学术论文
void insertAcademicPaper(StudentInfo *student, AcademicPaper *paper) {
    // 将新论文添加到学生的学术论文链表
    if (student == NULL || paper == NULL) {
        return; // 如果学生或论文节点为空，则不执行操作
    }

    AcademicPaper *current = student->academicPapers;
    if (current == NULL) {
        // 如果学生的论文链表为空，则新节点成为第一个节点
        student->academicPapers = paper;
    } else {
        // 遍历链表直到找到最后一个节点
        while (current->next != NULL) {
            current = current->next;
        }
        // 将新节点添加到链表的末尾
        current->next = paper;
    }
    printf("论文录入成功");
}

// 插入计算机类学科竞赛
void insertCompetition(StudentInfo *student, Competition *competition) {
    // 将新竞赛添加到学生的计算机类学科竞赛链表
    if (student == NULL || competition == NULL) {
        return; // 如果学生或竞赛节点为空，则不执行操作
    }

    Competition *current = student->competitions;
    if (current == NULL) {
        // 如果学生的竞赛链表为空，则新节点成为第一个节点
        student->competitions = competition;
    } else {
        // 遍历链表直到找到最后一个节点
        while (current->next != NULL) {
            current = current->next;
        }
        // 将新节点添加到链表的末尾
        current->next = competition;
    }
    printf("竞赛录入成功");
}

// 计算GPA、总学分、加权平均分、总绩点
void calculateGPA(StudentInfo *student) {
    // 计算并更新学生的GPA
    if (student == NULL) {
        return; // 如果传入的学生指针为空，则直接返回
    }
    float totalQualityPoints = 0.00f; // 用于累加课程的GPA值
    int numCourses = 0; // 用于计数课程数量
    float totalCredit = 0.00f;// 用于累加学分
    float totalScore = 0.00f; // 用于计算加权平均分


    // 遍历学生的学业成绩链表
    AcademicScoreNode *currentScore = student->academicScores;
    while (currentScore != NULL) {
        totalQualityPoints += currentScore->gpa * currentScore->credit; // 累加每个课程的GPA值和学分
        numCourses++;     // 课程数量计数
        totalCredit += currentScore->credit;    // 累加学分
        totalScore += currentScore->score * currentScore->credit; // 累加分数
        currentScore = currentScore->next; // 移动到下一个课程节点
    }

    // 总学分
    student->totalCredit = totalCredit;

    // 计算加权学业GPA
    if (numCourses > 0) {
        student->gpa = totalQualityPoints / totalCredit; // 计算学业GPA
    } else {
        student->gpa = 0.00f; // 如果没有课程，GPA为0
    }

    // 总绩点
    student->totalGPA = student->gpa + student->qualityGpa;

    // 计算加权平均分
    student->averageScore = totalScore / totalCredit;
}

// 计算单门课程绩点
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

// 删除学生记录
void deleteStudentRecord(StudentInfo **head, char *studentID) {
    // 根据学号删除学生记录
    // 使用findStudentByID函数查找学生
    StudentInfo *studentToDelete = findStudentByID(studentID);
    if (studentToDelete != NULL) {
        // 如果找到了学生，从链表中删除
        if (*head == studentToDelete) {
            // 如果要删除的是头节点
            *head = (*head)->next;
        } else {
            // 如果要删除的是链表中间或尾部的节点
            StudentInfo *current = *head;
            while (current->next != studentToDelete) {
                current = current->next;
            }
            current->next = current->next->next;
        }
        free(studentToDelete); // 释放学生记录的内存
        printf("学生记录删除成功。\n");
    } else {
        // 如果没有找到学生ID匹配的节点，打印错误信息
        printf("未找到学号为 %s 的学生记录。\n", studentID);
    }
}

// 处理输入错误
void handleInputError(const char *message) {
    // 显示错误信息
    printf("%s\n", message);
}

// 创建新学生
StudentInfo *createStudent() {
    // 分配内存给新的StudentInfo实例
    StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
    if (newStudent == NULL) {
        // 如果内存分配失败，打印错误信息并退出程序
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }

    // 初始化新学生信息
    newStudent->next = NULL; // 初始化链表指针
    newStudent->academicScores = NULL; // 初始化学业成绩链表
    newStudent->innovationProjects = NULL; // 初始化大学生创新创业计划项目链表
    newStudent->academicPapers = NULL; // 初始化学术论文链表
    newStudent->competitions = NULL; // 初始化计算机类学科竞赛链表
    newStudent->gpa = 0.00f; // 初始化GPA
    newStudent->totalCredit = 0; // 初始化总学分
    newStudent->qualityGpa = 0.00f; // 初始化素质加分GPA
    newStudent->averageScore = 0.00f; // 初始化加权平均分
    strcpy(newStudent->name, "New Student");
    strcpy(newStudent->studentID, "00000000");
    newStudent->classNumber = 1; // 假设学生班级为1
    newStudent->grade = 1; // 假设学生年级为1
    strcpy(newStudent->password, "123456"); // 默认密码为123456

    // 输入学生信息
    printf("请输入学生信息：\n");
    printf("学号: ");
    scanf("%s", newStudent->studentID);
    // 判断学号是否是8位数字
    while (!isStudentIDValid(newStudent->studentID)) {
        handleInputError("学号应为8位数字，请重新输入。\n");
        scanf("%s", newStudent->studentID);
    }
    // 检查学号是否已存在
    if (findStudentByID(newStudent->studentID) != NULL) {
        // 如果学号已存在，打印错误信息、释放内存并返回上一级菜单
        handleInputError("学号已存在，请重新输入。");
        free(newStudent);
        return NULL;
    }
    printf("姓名: ");
    scanf("%s", newStudent->name);
    printf("班级: ");
    scanf("%d", &newStudent->classNumber);
    //判断班级是否合法
    while (!isClassValid(newStudent->classNumber)) {
        handleInputError("班级应为大于0的数，请重新输入。\n");
        scanf("%d", &newStudent->classNumber);
    }

    printf("年级: ");
    scanf("%d", &newStudent->grade);
    //判断年级是否合法
    while (!isGradeValid(newStudent->grade)) {
        handleInputError("年级应为1-4之间的数(1.大一 2.大二 3.大三 4.大四)，请重新输入。\n");
        scanf("%d", &newStudent->grade);
    }

    //录入学业成绩
    int choice1;
    printf("是否现在录入课程记录？(1. 是 2. 否): ");
    scanf("%d", &choice1);
    //判断选择是否合法
    while (choice1 != 1 && choice1 != 2) {
        handleInputError("选项应为1或2，请重新输入。\n");
        scanf("%d", &choice1);
    }
    if(choice1 == 1){
        int numCourses;
        printf("请输入要输入的课程数量：");
        scanf("%d", &numCourses);
        for (int i = 0; i < numCourses; i++) {
            printf("请输入第%d门课程信息：\n", i + 1);
            addCourse(newStudent);
        }
        calculateGPA(newStudent);
    }

    int choice2;
    printf("是否现在录入素质加分项目记录？(1. 是 2. 否): ");
    scanf("%d", &choice2);
    //判断选择是否合法
    while (choice2 != 1 && choice2 != 2) {
        handleInputError("选项应为1或2，请重新输入。\n");
        scanf("%d", &choice2);
    }
    if(choice2 == 1){
        //录入素质加分项目
        addQualityProject(newStudent);
        calculateQualityGPA(newStudent);
        calculateGPA(newStudent);
    }
    // 返回新创建的StudentInfo指针
    return newStudent;
}

// 创建新学业成绩
AcademicScoreNode *createAcademicScore() {
        // 分配内存给新的AcademicScoreNode实例
        AcademicScoreNode *newCourse = (AcademicScoreNode *)malloc(sizeof(AcademicScoreNode));
        if (newCourse == NULL) {
            // 如果内存分配失败，打印错误信息并退出程序
            fprintf(stderr, "内存分配失败\n");
            exit(EXIT_FAILURE);
        }
        // 初始化新课程信息
        newCourse->next = NULL; // 初始化链表指针
        strcpy(newCourse->courseName, "Unknown Course"); // 初始化课程名称
        newCourse->score = 0.00f; // 初始化课程分数
        newCourse->credit = 0.0f; // 初始化课程学分
        newCourse->gpa = 0.0f; // 初始化课程绩点
        printf("请输入课程信息：\n");
        printf("课程名称: ");
        scanf("%s", newCourse->courseName);
        printf("分数: ");
        scanf("%f", &newCourse->score);
        // 判断分数是否合法
        while (!isScoreValid(newCourse->score)) {
            handleInputError("分数应为0-100之间的数，请重新输入。\n");
            scanf("%f", &newCourse->score);
        }
        printf("学分: ");
        scanf("%f", &newCourse->credit);
        // 判断学分是否合法
        while (!isCreditValid(newCourse->credit)) {
            handleInputError("学分应为小于8数，请重新输入。\n");
            scanf("%f", &newCourse->credit);
        }
        newCourse->gpa = calculateSingleGPA(&newCourse->score); // 计算课程绩点
        // 返回新创建的AcademicScoreNode指针
        return newCourse;
}

// 创建新大学生创新创业计划项目
InnovationProject *createInnovationProject() {
    InnovationProject *newProject = (InnovationProject *)malloc(sizeof(InnovationProject));
    if (newProject == NULL) {
        // 如果内存分配失败，打印错误信息并退出程序
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    newProject->gpa = 0.00f; // 默认加分值初始化为0.0
    printf("请输入大学生创新创业计划项目信息：\n");
    printf("项目名称: ");
    scanf("%s", newProject->projectName);
    printf("是否为项目负责人(0.否 1.第一负责人 2.第二负责人): ");
    scanf("%d", &newProject->isLeaderOrSecondLeader);
    // 判断是否为项目负责人是否合法
    while (!isLeaderOrSecondLeaderValid(newProject->isLeaderOrSecondLeader)) {
        handleInputError("是否为项目负责人应为0-2之间的数，请重新输入。\n");
        scanf("%d", &newProject->isLeaderOrSecondLeader);
    }
    printf("项目级别(国家级，省级，校级): ");
    scanf("%s", newProject->projectLevel);
    // 判断项目级别是否合法
    while (!isProjectLevelValid(newProject->projectLevel)) {
        handleInputError("项目级别应为国家级，省级，校级，请重新输入。\n");
        scanf("%s", newProject->projectLevel);
    }
    printf("是否结题(1. 是 2. 否): ");
    scanf("%d", &newProject->isFinished);
    // 判断是否结题是否合法
    while (newProject->isFinished != 1 && newProject->isFinished != 2) {
        handleInputError("是否结题应为1或2，请重新输入。\n");
        scanf("%d", &newProject->isFinished);
    }
    newProject->gpa = calculateInnovationProjectGPA(newProject);
    // 返回新创建的InnovationProject指针
    return newProject;
}

// 创建新学术论文
AcademicPaper *createAcademicPaper() {
    AcademicPaper *newPaper = (AcademicPaper *)malloc(sizeof(AcademicPaper));
    if (newPaper == NULL) {
        // 如果内存分配失败，打印错误信息并退出程序
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    newPaper->gpa = 0.00f; // 默认加分值初始化为0.0
    printf("请输入学术论文信息：\n");
    printf("论文标题: ");
    scanf("%s", newPaper->title);
    printf("期刊名称: ");
    scanf("%s", newPaper->journalName);
    printf("是否为第一作者(1. 是 2. 否): ");
    scanf("%d", &newPaper->isFirstAuthor);
    // 判断是否为第一作者是否合法
    while (newPaper->isFirstAuthor != 1 && newPaper->isFirstAuthor != 2) {
        handleInputError("是否为第一作者应为1或2，请重新输入。\n");
        scanf("%d", &newPaper->isFirstAuthor);
    }
    printf("期刊级别(1. 顶级 2. 一级 3. 二级 4. 三级 5. 四级 6. 五级): ");
    scanf("%d", &newPaper->journalLevel);
    // 判断期刊级别是否合法
    while (!isJournalLevelValid(newPaper->journalLevel)) {
        handleInputError("期刊级别应为1-6之间的数，请重新输入。\n");
        scanf("%d", &newPaper->journalLevel);
    }
    newPaper->gpa = calculatePaperGPA(newPaper);
    // 返回新创建的AcademicPaper指针
    return newPaper;
}

// 创建新计算机类学科竞赛
Competition *createCompetition() {
    Competition *newCompetition = (Competition *)malloc(sizeof(Competition));
    if (newCompetition == NULL) {
        // 如果内存分配失败，打印错误信息并退出程序
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    newCompetition->gpa = 0.00f; // 默认加分值初始化为0.0
    printf("请输入计算机类学科竞赛信息：\n");
    printf("竞赛名称: ");
    scanf("%s", newCompetition->competitionName);
    printf("参赛队伍人数: ");
    scanf("%d", &newCompetition->teamSize);
    printf("竞赛级别(国家级，省级，校级): ");
    scanf("%s", newCompetition->competitionLevel);
    // 判断竞赛级别是否合法
    while (!isCompetitionLevelValid(newCompetition->competitionLevel)) {
        handleInputError("竞赛级别应为国家级，省级，校级，请重新输入。\n");
        scanf("%s", newCompetition->competitionLevel);
    }
    printf("获奖等级(1. 一等奖 2. 二等奖 3. 三等奖): ");
    scanf("%d", &newCompetition->rank);
    // 判断获奖等级是否合法
    while (!isRankValid(newCompetition->rank)) {
        handleInputError("获奖等级应为1-3之间的数，请重新输入。\n");
        scanf("%d", &newCompetition->rank);
    }
    printf("竞赛类别(A类，B类，C类): ");
    scanf("%s", newCompetition->competitionType);
    // 判断竞赛类别是否合法
    while (!isCompetitionTypeValid(newCompetition->competitionType)) {
        handleInputError("竞赛类别应为A类，B类，C类，请重新输入。\n");
        scanf("%c", newCompetition->competitionType);
    }
    newCompetition->gpa = calculateCompetitionGPA(newCompetition);
    // 返回新创建的Competition指针
    return newCompetition;
}

// 按学号查找指定学生
StudentInfo *findStudentByID(const char *studentID) {
    // 遍历学生列表，查找指定学号的学生
    // 从学生列表的头部开始遍历
    StudentInfo *currentStudent = studentsList;

    // 遍历整个学生列表
    while (currentStudent != NULL) {
        // 比较当前学生的学号与给定的学号
        if (strcmp(currentStudent->studentID, studentID) == 0) {
            // 如果找到匹配的学号，返回当前学生的信息
            return currentStudent;
        }
        // 移动到链表的下一个节点
        currentStudent = currentStudent->next;
    }
    // 如果遍历完整个列表都没有找到匹配的学号，返回NULL
    return NULL;
}

// 查找管理员
AdminInfo *findAdminByUsername(const char *username) {
    // 比较给定的用户名和管理员的用户名
    if (strcmp(adminsList->username, username) == 0) {
        // 如果找到匹配的用户名，返回管理员信息
        return adminsList;
    }
    // 如果没有找到匹配的用户名，返回NULL
    return NULL;
}

// 编辑学生学业成绩
void modifyAcademicScore(StudentInfo *student) {
    // 修改学业成绩
    int choice;
    printf("请选择修改类型：\n");
    printf("1. 修改课程名称\n");
    printf("2. 修改分数\n");
    printf("3. 修改学分\n");
    printf("4. 删除课程\n");
    printf("5. 返回上一级菜单\n");
    scanf("%d", &choice);
    //判断选择是否合法
    while (choice < 1 || choice > 5) {
        handleInputError("选项应为1-5之间的数，请重新输入。\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:{
            // 修改课程名称
            if (student->academicScores == NULL) {
                printf("当前没有课程可以修改。\n");
                break;
            }
            AcademicScoreNode *current = student->academicScores;
            printf("请输入要修改的课程名称（输入q退出）：");
            char courseName[MAX_NAME_LENGTH];
            scanf("%s", courseName);
            if (strcmp(courseName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->courseName, courseName) == 0) {
                    printf("找到课程: %s, 当前分数: %.2f\n", current->courseName, current->score);
                    printf("请输入新的课程名称: ");
                    scanf("%s", current->courseName);
                    printf("课程名称修改成功。\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("未找到指定的课程。\n");
            }
            break;
        }
        case 2:{
            // 修改分数
            if (student->academicScores == NULL) {
                printf("当前没有课程可以修改。\n");
                break;
            }
            AcademicScoreNode *currentScore = student->academicScores;
            printf("请输入要修改的课程名称（输入q退出）：");
            char scoreCourseName[MAX_NAME_LENGTH];
            scanf("%s", scoreCourseName);
            if (strcmp(scoreCourseName, "q") == 0) {
                break;
            }
            while (currentScore != NULL) {
                if (strcmp(currentScore->courseName, scoreCourseName) == 0) {
                    printf("找到课程: %s, 当前分数: %f\n", currentScore->courseName, currentScore->score);
                    printf("请输入新的分数: ");
                    scanf("%f", &currentScore->score);
                    //判断分数是否合法
                    while (!isScoreValid(currentScore->score)) {
                        handleInputError("分数应为0-100之间的数，请重新输入。\n");
                        scanf("%f", &currentScore->score);
                    }
                    currentScore->gpa = calculateSingleGPA(&currentScore->score);//重新计算当前课程绩点
                    calculateGPA(student);//重新计算GPA\总学分\加权平均分\总绩点
                    printf("分数修改成功。\n");
                    break;
                }
                currentScore = currentScore->next;
            }
            if (currentScore == NULL) {
                printf("未找到指定的课程。\n");
            }
            break;
        }
        case 3:{
            // 修改学分
            if (student->academicScores == NULL) {
                printf("当前没有课程可以修改。\n");
                break;
            }
            AcademicScoreNode *currentCredit = student->academicScores;
            printf("请输入要修改的课程名称（输入q退出）：");
            char creditCourseName[MAX_NAME_LENGTH];
            scanf("%s", creditCourseName);
            if (strcmp(creditCourseName, "q") == 0) {
                break;
            }
            while (currentCredit != NULL) {
                if (strcmp(currentCredit->courseName, creditCourseName) == 0) {
                    printf("找到课程: %s, 当前学分: %.2f\n", currentCredit->courseName, currentCredit->credit);
                    printf("请输入新的学分: ");
                    scanf("%2f", &currentCredit->credit);
                    //判断学分是否合法
                    while (!isCreditValid(currentCredit->credit)) {
                        handleInputError("学分应为小于8数，请重新输入。\n");
                        scanf("%2f", &currentCredit->credit);
                    }
                    calculateGPA(student);  //重新计算GPA\总学分\加权平均分\总绩点
                    printf("学分修改成功。\n");
                    break;
                }
                currentCredit = currentCredit->next;
            }
            if (currentCredit == NULL) {
                printf("未找到指定的课程。\n");
            }
            break;
        }
        case 4:{
            // 删除课程
            if (student->academicScores == NULL) {
                printf("当前没有课程可以删除。\n");
                break;
            }
            AcademicScoreNode *currentDelete = student->academicScores;
            AcademicScoreNode *prev = NULL;
            printf("请输入要删除的课程名称（输入q退出）：");
            char deleteCourseName[MAX_NAME_LENGTH];
            scanf("%s", deleteCourseName);
            if (strcmp(deleteCourseName, "q") == 0) {
                break;
            }
            while (currentDelete != NULL) {
                if (strcmp(currentDelete->courseName, deleteCourseName) == 0) {
                    if (prev == NULL) {
                        // 要删除的是头节点
                        student->academicScores = currentDelete->next;
                    } else {
                        // 要删除的是中间或尾节点
                        prev->next = currentDelete->next;
                    }
                    AcademicScoreNode *temp = currentDelete;
                    currentDelete = currentDelete->next;
                    free(temp); // 释放被删除节点的内存
                    calculateGPA(student);  //重新计算GPA\总学分\加权平均分\总绩点
                    printf("课程删除成功。\n");
                    break;
                }
                prev = currentDelete;
                currentDelete = currentDelete->next;
            }
            if (currentDelete == NULL) {
                printf("未找到指定的课程。\n");
            }
            break;
        }
        case 5:{
            printf("返回上一级菜单。\n");
            // 返回上一级菜单
            break;
        }
        default: {
            printf("无效的选项。\n");
        }
    }
}

// 编辑学生素质加分
void modifyQualityScore(StudentInfo *student) {
    // 修改素质加分
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }

    int choice;
    printf("请选择修改类型：\n");
    printf("1. 修改大学生创新创业项目\n");
    printf("2. 删除大学生创新创业项目\n");
    printf("3. 修改学术论文\n");
    printf("4. 删除学术论文\n");
    printf("5. 修改计算机类学科竞赛\n");
    printf("6. 删除计算机类学科竞赛\n");
    printf("7. 返回上一级菜单\n");
    scanf("%d", &choice);
    // 判断选择是否合法
    while (choice < 1 || choice > 7) {
        handleInputError("选项应为1-7之间的数，请重新输入。\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:{
            // 修改大学生创新创业项目
            modifyInnovationProject(student);
            break;
        }
        case 2:{
            // 删除大学生创新创业项目
            deleteInnovationProject(student);
            break;
        }
        case 3:{
            // 修改学术论文
            modifyAcademicPaper(student);
            break;
        }
        case 4:{
            // 删除学术论文
            deleteAcademicPaper(student);
            break;
        }
        case 5:{
            // 修改计算机类学科竞赛
            modifyCompetition(student);
            break;
        }
        case 6:{
            // 删除计算机类学科竞赛
            deleteCompetition(student);
            break;
        }
        case 7:{
            printf("返回上一级菜单。\n");
            // 返回上一级菜单
            break;
        }
        default: {
            printf("无效的选项。\n");
        }

    }
}

// 显示指定学生的全部学业成绩
void displayAcademicScore(StudentInfo *student) {
    // 显示学生的学业成绩
    if (student == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    AcademicScoreNode *current = student->academicScores;
    if (current == NULL) {
        printf("学生没有学业成绩。\n");
        return;
    }
    printf("学业成绩：\n");
    while (current != NULL) {
        printf("课程名称: %s, 学分: %.2f, 分数: %.2f, 绩点: %.2f\n", current->courseName, current->credit, current->score, current->gpa);
        current = current->next;
    }
    printf("所有学业成绩已全部显示完成。\n");
}

// 显示指定学生的全部素质加分
void displayQualityScore(StudentInfo *student) {
    // 显示学生的素质加分
    if (student == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    displayInnovationProjects(student);//显示大学生创新创业计划项目
    displayAcademicPapers(student);//显示学术论文
    displayCompetitions(student);//显示计算机类学科竞赛
}

//释放管理员内存
void freeAdminMemory(AdminInfo **head) {
    // 检查管理员列表头指针是否为空
    if (*head == NULL) {
        return; // 如果头指针为空，则没有内存需要释放
    }
    // 释放当前管理员节点的内存
    AdminInfo *tempAdmin = *head;
    *head = NULL;
    free(tempAdmin); // 释放节点内存
}

//显示所有学生概况
void displayAllStudents(StudentInfo *head) {
    // 显示所有学生的信息
    if (head == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    StudentInfo *current = head;
    while (current != NULL) {
        printf("学号: %s\n", current->studentID);
        printf("姓名: %s\n", current->name);
        printf("年级: %d\n", current->grade);
        printf("班级: %d\n", current->classNumber);
        printf("已获总学分: %.2f\n", current->totalCredit);
        printf("学业GPA: %f\n", current->gpa);
        printf("素质加分GPA: %.2f\n", current->qualityGpa);
        printf("加权平均分: %.2f\n", current->averageScore);
        printf("总绩点: %.2f\n", current->totalGPA);
        printf("\n\n");
        current = current->next;
    }
    printf("\n");
    printf("所有学生信息已全部显示完成。\n");
}

//排序学生链表
void sortList(StudentInfo **head, int (*compare)(const StudentInfo *, const StudentInfo *)) {
    int swapped;
    StudentInfo *current;
    StudentInfo *prev = NULL;
    do {
        swapped = 0;
        current = *head;
        while (current && current->next) {
            if ((*compare)(current, current->next) > 0) {
                // 交换两个节点的数据
                StudentInfo temp = *current;
                *current = *(current->next);
                *(current->next) = temp;
                swapped = 1;
            }
            current = current->next;
        }
    } while (swapped);
}

// 比较函数：按学号排序
int compareByStudentID(const StudentInfo *a, const StudentInfo *b) {
    return strcmp(a->studentID, b->studentID);
}

// 比较函数：按学业GPA排序
int compareByGPA(const StudentInfo *a, const StudentInfo *b) {
    if (a->gpa > b->gpa) return -1;
    if (a->gpa < b->gpa) return 1;
    return 0;
}

// 比较函数：按加权平均分排序
int compareByAverageScore(const StudentInfo *a, const StudentInfo *b) {
    if (a->averageScore > b->averageScore) return -1;
    if (a->averageScore < b->averageScore) return 1;
    return 0;
}

// 比较函数：按总绩点排序
int compareByTotalGPA(const StudentInfo *a, const StudentInfo *b) {
    if (a->totalGPA > b->totalGPA) return -1;
    if (a->totalGPA < b->totalGPA) return 1;
    return 0;
}

// 添加管理员账号
void addAdminAccount() {
    AdminInfo *newAdmin = (AdminInfo *)malloc(sizeof(AdminInfo)); // 分配新管理员内存
    if (newAdmin == NULL) {
        handleInputError("内存分配失败，无法添加管理员账号。\n");
        return;
    }

    printf("请输入管理员用户名: ");
    scanf("%s", newAdmin->username);
    // 检查用户名是否已存在
    while (findAdminByUsername(newAdmin->username) != NULL) {
        handleInputError("用户名已存在，请重新输入: ");
        scanf("%s", newAdmin->username);
    }

    printf("请输入管理员密码: ");
    scanf("%s", newAdmin->password);
    // 检查密码是否合法
    while (!isPasswordValid(newAdmin->password)) {
        handleInputError("密码应为1-15位字符，请重新输入: ");
        scanf("%s", newAdmin->password);
    }

    // 将新管理员添加到链表头部
    newAdmin->next = adminsList;
    adminsList = newAdmin;

    printf("管理员账号添加成功。\n");
}

// 保存到文件
void saveToFile() {
    FILE *file = fopen("student.txt", "w");
    if (file == NULL) {
        handleInputError("打开文件失败");
        return;
    }

    StudentInfo *currentStudent = studentsList;
    while (currentStudent != NULL) {
        // 学生记录开始
        fprintf(file, "%s\n", STUDENT_START);

//        char studentID[8];// 学号
//        char name[15];// 姓名
//        char password[15];// 密码
//        int classNumber;// 班级
//        float gpa;  // 学业GPA
//        float totalCredit;// 总学分
//        float qualityGpa;// 素质加分GPA
//        float averageScore;// 加权平均分
//        float totalGPA;// 总绩点

        // 写入学生个人信息
        fprintf(file, "StudentID: %s\n", currentStudent->studentID);// 学号
        fprintf(file, "Name: %s\n", currentStudent->name);// 姓名
        fprintf(file, "Password: %s\n", currentStudent->password);// 密码
        fprintf(file, "Grade: %d\n", currentStudent->grade);// 年级
        fprintf(file, "ClassNumber: %d\n", currentStudent->classNumber);// 班级
        fprintf(file, "GPA: %f\n", currentStudent->gpa);// 学业GPA
        fprintf(file, "TotalCredit: %.2f\n", currentStudent->totalCredit);// 总学分
        fprintf(file, "QualityGPA: %.2f\n", currentStudent->qualityGpa);// 素质加分GPA
        fprintf(file, "AverageScore: %f\n", currentStudent->averageScore);// 加权平均分
        fprintf(file, "TotalGPA: %f\n", currentStudent->totalGPA);// 总绩点

//        char courseName[MAX_NAME_LENGTH];
//        float score;    // 分数
//        float credit; // 学分
//        float gpa; // 单门课程绩点
        // 学业成绩链表
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

//        char projectName[MAX_NAME_LENGTH]; // 项目名称
//        float gpa; // 加分值
//        int isLeaderOrSecondLeader;// 是否为项目负责人,是否为第二负责人，国家级优秀结题项目负责人加0.1 GPA，第二名加0.05 GPA
//        char projectLevel[10];// 项目级别，国家级，省级，校级
//        int isFinished;    // 是否结题
        // 大学生创新创业计划项目链表
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

//        char title[MAX_NAME_LENGTH]; // 论文标题
//        char journalName[MAX_NAME_LENGTH]; // 期刊名称
//        int isFirstAuthor; // 是否为第一作者
//
//        //期刊级别，顶级加0.4，一级期刊加0.2 GPA，二级期刊加0.1 GPA，三级期刊加0.05 GPA，四级期刊加0.02 GPA，五级期刊加0.01 GPA
//        int journalLevel;
//
//        float gpa; // 加分值
        // 学术论文链表
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

//        char competitionName[MAX_NAME_LENGTH]; // 竞赛名称
//        int teamSize;   //参赛队伍人数
//        char competitionLevel[10]; // 竞赛级别,国家级，省级，校级
//        int rank;   // 获奖等级，1表示一等奖，2表示二等奖，3表示三等奖
//        char competitionType;   //竞赛类别，A类，B类，C类
//        float gpa; // 加分值
        // 计算机类学科竞赛链表
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

        // 学生记录结束
        fprintf(file, "%s\n", STUDENT_END);

        currentStudent = currentStudent->next;
    }
    fclose(file);//关闭文件

    // 保存管理员信息
    FILE *adminFile = fopen("admin.txt", "w");
    if (adminFile == NULL) {
        handleInputError("打开文件失败");
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
    fclose(adminFile);//关闭文件
}

// 从文件加载
void loadFromFile() {
    FILE *file = fopen("student.txt", "r");
    if (file == NULL) {
        handleInputError("打开文件失败");
        return;
    }

    freeMemory(&studentsList); // 清空学生链表

    char buffer[1024];// 用于存储读取的每行数据
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, STUDENT_START, strlen(STUDENT_START)) == 0) {
            // 创建新的学生节点
            StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
            if (newStudent == NULL) {
                handleInputError("内存分配失败");
                break;
            }
            memset(newStudent, 0, sizeof(StudentInfo));

            // 读取学生个人信息
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

            // 将新节点添加到学生链表中
            insertStudent(&studentsList, newStudent);

            // 读取学业成绩链表
            while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, SCORE_END, strlen(SCORE_END)) != 0) {
                AcademicScoreNode *newScore = (AcademicScoreNode *)malloc(sizeof(AcademicScoreNode));
                if (newScore == NULL) {
                    handleInputError("内存分配失败");
                    break;
                }
                memset(newScore, 0, sizeof(AcademicScoreNode));
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "CourseName: %s", newScore->courseName);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "Score: %f", &newScore->score);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "Credit: %2f", &newScore->credit);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "GPA: %2f", &newScore->gpa);
                newScore->next = newStudent->academicScores;
                newStudent->academicScores = newScore;
            }

            // 读取大学生创新创业计划项目链表
            if (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, PROJECT_START, strlen(PROJECT_START)) == 0) {
                InnovationProject *newProject = (InnovationProject *)malloc(sizeof(InnovationProject));
                if (newProject == NULL) {
                    handleInputError("内存分配失败");
                    break;
                }
                memset(newProject, 0, sizeof(InnovationProject));
                newStudent->innovationProjects = newProject;

                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "ProjectName: %s", newProject->projectName);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "GPA: %2f", &newProject->gpa);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "LeaderOrSecondLeader: %d", &newProject->isLeaderOrSecondLeader);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "ProjectLevel: %s", newProject->projectLevel);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "IsFinished: %d", &newProject->isFinished);
            }

            // 读取学术论文链表
            while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, PAPER_END, strlen(PAPER_END)) != 0) {
                AcademicPaper *newPaper = (AcademicPaper *)malloc(sizeof(AcademicPaper));
                if (newPaper == NULL) {
                    handleInputError("内存分配失败");
                    break;
                }
                memset(newPaper, 0, sizeof(AcademicPaper));
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "Title: %s", newPaper->title);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "JournalName: %s", newPaper->journalName);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "IsFirstAuthor: %d", &newPaper->isFirstAuthor);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "JournalLevel: %d", &newPaper->journalLevel);
                fgets(buffer, sizeof(buffer), file);
                sscanf(buffer, "GPA: %2f", &newPaper->gpa);
                newPaper->next = newStudent->academicPapers;
                newStudent->academicPapers = newPaper;
            }

            // 读取计算机类学科竞赛链表
            while (fgets(buffer, sizeof(buffer), file) && strncmp(buffer, COMPETITION_END, strlen(COMPETITION_END)) != 0) {
                Competition *newCompetition = (Competition *)malloc(sizeof(Competition));
                if (newCompetition == NULL) {
                    handleInputError("内存分配失败");
                    break;
                }
                memset(newCompetition, 0, sizeof(Competition));
                fgets(buffer, sizeof(buffer), file);
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
                sscanf(buffer, "GPA: %2f", &newCompetition->gpa);
                newCompetition->next = newStudent->competitions;
                newStudent->competitions = newCompetition;
            }
            // 跳过学生记录结束标记
            fgets(buffer, sizeof(buffer), file);
        }
    }
    fclose(file); // 关闭文件

    // 加载管理员信息
    FILE *adminFile = fopen("admin.txt", "r");
    if (adminFile == NULL) {
        handleInputError("打开文件失败");
        return;
    }
    freeAdminMemory(&adminsList); // 清空管理员链表

    while (fgets(buffer, sizeof(buffer), adminFile)) {
        if (strncmp(buffer, ADMIN_START, strlen(ADMIN_START)) == 0) {
            AdminInfo *newAdmin = (AdminInfo *)malloc(sizeof(AdminInfo));
            if (newAdmin == NULL) {
                handleInputError("内存分配失败");
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
    fclose(adminFile); // 关闭文件
}

// 计算并更新学生的素质加分GPA
void calculateQualityGPA(StudentInfo *student) {
    // 计算并更新学生的素质加分GPA

    if (student == NULL) {
        return; // 如果传入的学生指针为空，则直接返回
    }
    float totalQualityPoints = 0.00f; // 用于累加素质加分GPA值
    float gpa1 = 0.00f; // 用于大学生创新创业项目加分GPA
    float gpa2 = 0.00f; // 用于学术论文所加最大值
    float gpa3 = 0.00f; // 用于大学生创新创业计划项目所加最大值

    // 学生的大学生创新创业计划项目链表
    InnovationProject *currentProject = student->innovationProjects;
    if (currentProject != NULL) {
        gpa1 = currentProject->gpa; // 累加每个项目的GPA值
    }

    // 学生的学术论文链表
    AcademicPaper *currentPaper = student->academicPapers;
    while (currentPaper != NULL) {
        if(currentPaper->gpa > gpa2){
            gpa2 = currentPaper->gpa;
        }
        currentPaper = currentPaper->next; // 移动到下一个论文节点
    }

    // 学生的计算机类学科竞赛链表
    Competition *currentCompetition = student->competitions;
    while (currentCompetition != NULL) {
        if(currentCompetition->gpa > gpa3){
            gpa3 = currentCompetition->gpa;
        }
        currentCompetition = currentCompetition->next; // 移动到下一个竞赛节点
    }
    totalQualityPoints = gpa1 + gpa2 + gpa3;
    student->qualityGpa = totalQualityPoints; // 总素质加分GPA
}

// 计算大学生创新创业计划项目加分GPA
float calculateInnovationProjectGPA(InnovationProject *project) {
    // 根据项目级别和是否结题计算加分GPA
    float gpa = 0.00f;
    if (project == NULL) {
        return gpa; // 如果传入的项目指针为空，则直接返回
    }
if (project->isFinished == 1) {
        // 如果项目已结题，根据项目级别计算加分GPA
        if (strcmp(project->projectLevel, "国家级") == 0) {
            //第一负责人
            if (project->isLeaderOrSecondLeader == 1) {
                gpa = 0.10f;
            } else if(project->isLeaderOrSecondLeader == 2) {
                gpa = 0.05f;
            }
        }
    }
    return gpa;
}

// 计算学术论文加分GPA
float calculatePaperGPA(AcademicPaper *paper) {
    // 根据期刊级别和是否为第一作者计算加分GPA
    float gpa = 0.00f;
    if (paper == NULL) {
        return gpa; // 如果传入的论文指针为空，则直接返回
    }
    if (paper->isFirstAuthor == 1) {
        // 如果是第一作者，根据期刊级别计算加分GPA
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

// 计算计算机类学科竞赛加分GPA
float calculateCompetitionGPA(Competition *competition) {
    // 根据获奖等级和竞赛级别计算加分GPA
    float gpa = 0.00f;
    if (competition == NULL) {
        return gpa; // 如果传入的竞赛指针为空，则直接返回
    }
    //如果是国家级
    if (strcmp(competition->competitionLevel, "国家级") == 0) {

        if (competition->competitionType=='A' && competition->rank == 1) {
            //A类一等奖
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

// 显示学生的大学生创新创业计划项目
void displayInnovationProjects(StudentInfo *student) {
    // 显示学生的大学生创新创业计划项目
    if (student == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    InnovationProject *current = student->innovationProjects;
    if (current == NULL) {
        printf("学生没有大学生创新创业计划项目。\n");
        return;
    }else{
        printf("大学生创新创业计划项目：\n");
        printf("项目名称: %s, 是否结题: %d, 项目级别: %s, 是否为项目负责人: %d, 加分: %.2f\n", current->projectName, current->isFinished, current->projectLevel, current->isLeaderOrSecondLeader, current->gpa);
    }
}

// 显示学生的全部学术论文
void displayAcademicPapers(StudentInfo *student) {
    // 显示学生的学术论文
    if (student == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    AcademicPaper *current = student->academicPapers;
    if (current == NULL) {
        printf("学生没有学术论文。\n");
        return;
    }
    printf("学术论文：\n");
    while (current != NULL) {
        printf("论文标题: %s, 期刊名称: %s, 是否第一作者: %d, 期刊级别: %d, 加分: %.2f\n", current->title, current->journalName, current->isFirstAuthor, current->journalLevel, current->gpa);
        current = current->next;
    }
    printf("所有学术论文已全部显示完成。\n");
}

// 显示学生的全部计算机类学科竞赛
void displayCompetitions(StudentInfo *student) {
    // 显示学生的计算机类学科竞赛
    if (student == NULL) {
        printf("学生信息为空。\n");
        return;
    }
    Competition *current = student->competitions;
    if (current == NULL) {
        printf("该学生没有计算机类学科竞赛。\n");
        return;
    }
    printf("计算机类学科竞赛：\n");
    while (current != NULL) {
        printf("竞赛名称: %s, 参赛队伍人数: %d, 竞赛级别: %s, 获奖等级: %d, 竞赛类别: %s, 加分: %.2f\n", current->competitionName, current->teamSize, current->competitionLevel, current->rank, current->competitionType, current->gpa);
        current = current->next;
    }
    printf("所有计算机类学科竞赛已全部显示完成。\n");
}

//修改大学生创新创业计划项目
void modifyInnovationProject(StudentInfo *student) {
    // 修改大学生创新创业计划项目
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    if (student->innovationProjects == NULL) {
        printf("当前没有项目可以修改。\n");
    }
    int choice;
    printf("请选择修改类型：\n");
    printf("1. 修改项目名称\n");
    printf("2. 修改项目级别\n");
    printf("3. 修改是否结题\n");
    printf("4. 修改是否为项目负责人\n");
    printf("5. 返回上一级菜单\n");
    scanf("%d", &choice);
    // 判断用户选择是否合法
    while (choice < 1 || choice > 5) {
        handleInputError("选项应在1-5之间，请重新输入：");
        return;
    }
    switch (choice) {
        case 1: {
            // 修改项目名称
            InnovationProject *current = student->innovationProjects;
            printf("请输入要修改的项目名称（输入q退出）：");
            char projectName[MAX_NAME_LENGTH];
            scanf("%s", projectName);
            if (strcmp(projectName, "q") == 0) {
                break;
            }
            strcpy(current->projectName, projectName);
            printf("项目名称修改成功。\n");
            break;
        }
        case 2: {
            // 修改项目级别
            InnovationProject *current = student->innovationProjects;
            printf("请输入要修改的级别（输入q退出）：");
            char projectLevel[MAX_NAME_LENGTH];
            scanf("%s", projectLevel);
            if (strcmp(projectLevel, "q") == 0) {
                break;
            }
            //判断输入是否合法
            while(isProjectLevelValid(projectLevel)){
                handleInputError("级别应为国家级，省级，校级，请重新输入：");
                scanf("%s", projectLevel);
            }
            strcpy(current->projectLevel, projectLevel);
            printf("项目级别修改成功。\n");
            break;
        }
        case 3: {
            // 修改是否结题
            InnovationProject *current = student->innovationProjects;
            printf("请输入是否结题（1. 是 2. 否）：");
            int isFinished;
            scanf("%d", &isFinished);
            //判断输入是否合法
            while(isFinished != 1 && isFinished != 2){
                handleInputError("输入应为1或2，请重新输入：");
                scanf("%d", &isFinished);
            }
            current->isFinished = isFinished;
            printf("是否结题修改成功。\n");
            break;
        }
        case 4: {
            // 修改是否为项目负责人
            InnovationProject *current = student->innovationProjects;
            printf("请输入是否为项目负责人（0.不是负责人 1. 第一负责人 2. 第二负责人）：");
            int isLeaderOrSecondLeader;
            scanf("%d", &isLeaderOrSecondLeader);
            //判断输入是否合法
            while(isLeaderOrSecondLeader != 0 && isLeaderOrSecondLeader != 1 && isLeaderOrSecondLeader != 2){
                handleInputError("输入应为0，1或2，请重新输入：");
                scanf("%d", &isLeaderOrSecondLeader);
            }
            current->isLeaderOrSecondLeader = isLeaderOrSecondLeader;
            printf("是否为项目负责人修改成功。\n");
            break;
        }
    }
}

//修改学术论文
void modifyAcademicPaper(StudentInfo *student) {
    // 修改学术论文
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    if (student->academicPapers == NULL) {
        printf("当前没有学术论文可以修改。\n");
    }
    int choice;
    printf("请选择修改类型：\n");
    printf("1. 修改论文标题\n");
    printf("2. 修改期刊名称\n");
    printf("3. 修改是否为第一作者\n");
    printf("4. 修改期刊级别\n");
    printf("5. 返回上一级菜单\n");
    scanf("%d", &choice);
    // 判断用户选择是否合法
    while (choice < 1 || choice > 5) {
        handleInputError("选项应在1-5之间，请重新输入：");
        return;
    }
    switch (choice) {
        case 1: {
            // 修改论文标题
            AcademicPaper *current = student->academicPapers;
            printf("请输入要修改的论文标题（输入q退出）：");
            char title[MAX_NAME_LENGTH];
            scanf("%s", title);
            if (strcmp(title, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->title, title) == 0) {
                    printf("找到论文: %s, 当前期刊名称: %s\n", current->title, current->journalName);
                    printf("请输入新的论文标题: ");
                    scanf("%s", current->title);
                    printf("论文标题修改成功。\n");
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 2: {
            // 修改期刊名称
            AcademicPaper *current = student->academicPapers;
            printf("请输入要修改的论文标题（输入q退出）：");
            char title[MAX_NAME_LENGTH];
            scanf("%s", title);
            if (strcmp(title, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->title, title) == 0) {
                    printf("找到论文: %s, 当前期刊名称: %s\n", current->title, current->journalName);
                    printf("请输入新的期刊名称: ");
                    scanf("%s", current->journalName);
                    printf("期刊名称修改成功。\n");
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 3: {
            // 修改是否为第一作者
            AcademicPaper *current = student->academicPapers;
            printf("请输入要修改的论文标题（输入q退出）：");
            char title[MAX_NAME_LENGTH];
            scanf("%s", title);
            if (strcmp(title, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->title, title) == 0) {
                    printf("找到论文: %s, 当前是否为第一作者: %d\n", current->title, current->isFirstAuthor);
                    printf("请输入是否为第一作者（0. 否 1. 是）: ");
                    int isFirstAuthor;
                    scanf("%d", &isFirstAuthor);
                    //判断输入是否合法
                    while(isFirstAuthor != 0 && isFirstAuthor != 1){
                        handleInputError("输入应为0或1，请重新输入：");
                        scanf("%d", &isFirstAuthor);
                    }
                    current->isFirstAuthor = isFirstAuthor;
                    printf("是否为第一作者修改成功。\n");
                    break;
                }
                current = current->next;
            }
            break;
        }
        case 4: {
            // 修改期刊级别
            AcademicPaper *current = student->academicPapers;
            printf("请输入要修改的论文标题（输入q退出）：");
            char title[MAX_NAME_LENGTH];
            scanf("%s", title);
            if (strcmp(title, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->title, title) == 0) {
                    printf("找到论文: %s, 当前期刊级别: %d\n", current->title, current->journalLevel);
                    printf("请输入新的期刊级别: ");
                    int journalLevel;
                    scanf("%d", &journalLevel);
                    //判断输入是否合法
                    while(isJournalLevelValid(journalLevel)){
                        handleInputError("级别应为1-6，请重新输入：");
                        scanf("%d", &journalLevel);
                    }
                    current->journalLevel = journalLevel;
                    printf("期刊级别修改成功。\n");
                    break;
                }
                current = current->next;
            }
            break;
        }
    }
}

//修改计算机类学科竞赛
void modifyCompetition(StudentInfo *student) {
    // 修改计算机类学科竞赛
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    if (student->competitions == NULL) {
        printf("当前没有竞赛可以修改。\n");
    }
    int choice;
    printf("请选择修改类型：\n");
    printf("1. 修改竞赛名称\n");
    printf("2. 修改参赛队伍人数\n");
    printf("3. 修改竞赛级别\n");
    printf("4. 修改获奖等级\n");
    printf("5. 修改竞赛类别\n");
    printf("6. 返回上一级菜单\n");
    scanf("%d", &choice);
    // 判断用户选择是否合法
    while (choice < 1 || choice > 6) {
        handleInputError("输入错误，请重新输入。\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1: {
            // 修改竞赛名称
            Competition *current = student->competitions;
            printf("请输入要修改的竞赛名称（输入q退出）：");
            char competitionName[MAX_NAME_LENGTH];
            scanf("%s", competitionName);
            if (strcmp(competitionName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->competitionName, competitionName) == 0) {
                    printf("找到竞赛: %s, 当前参赛队伍人数: %d\n", current->competitionName, current->teamSize);
                    printf("请输入新的竞赛名称: ");
                    scanf("%s", current->competitionName);
                    printf("竞赛名称修改成功。\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("未找到指定的竞赛。\n");
            }
            break;
        }
        case 2: {
            // 修改参赛队伍人数
            Competition *current = student->competitions;
            printf("请输入要修改的竞赛名称（输入q退出）：");
            char competitionName[MAX_NAME_LENGTH];
            scanf("%s", competitionName);
            if (strcmp(competitionName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->competitionName, competitionName) == 0) {
                    printf("找到竞赛: %s, 当前参赛队伍人数: %d\n", current->competitionName, current->teamSize);
                    printf("请输入新的参赛队伍人数: ");
                    scanf("%d", &current->teamSize);
                    printf("参赛队伍人数修改成功。\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("未找到指定的竞赛。\n");
            }
            break;
        }
        case 3: {
            // 修改竞赛级别
            Competition *current = student->competitions;
            printf("请输入要修改的竞赛名称（输入q退出）：");
            char competitionName[MAX_NAME_LENGTH];
            scanf("%s", competitionName);
            if (strcmp(competitionName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->competitionName, competitionName) == 0) {
                    printf("找到竞赛: %s, 当前竞赛级别: %s\n", current->competitionName, current->competitionLevel);
                    printf("请输入新的竞赛级别: ");
                    scanf("%s", current->competitionLevel);
                    //判断输入是否合法
                    while(isCompetitionLevelValid(current->competitionLevel)){
                        handleInputError("级别应为国家级，省级，校级，请重新输入：");
                        scanf("%s", current->competitionLevel);
                    }
                    printf("竞赛级别修改成功。\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("未找到指定的竞赛。\n");
            }
            break;
        }
        case 4: {
            // 修改获奖等级
            Competition *current = student->competitions;
            printf("请输入要修改的竞赛名称（输入q退出）：");
            char competitionName[MAX_NAME_LENGTH];
            scanf("%s", competitionName);
            if (strcmp(competitionName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->competitionName, competitionName) == 0) {
                    printf("找到竞赛: %s, 当前获奖等级: %d\n", current->competitionName, current->rank);
                    printf("请输入新的获奖等级: (1, 2, 3)");
                    int rank;
                    scanf("%d", &rank);
                    //判断输入是否合法
                    while(isRankValid(rank)){
                        handleInputError("等级应为1，2，3，请重新输入：");
                        scanf("%d", &rank);
                    }
                    current->rank = rank;
                    printf("获奖等级修改成功。\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("未找到指定的竞赛。\n");
            }
            break;
        }
        case 5: {
            // 修改竞赛类别
            Competition *current = student->competitions;
            printf("请输入要修改的竞赛名称（输入q退出）：");
            char competitionName[MAX_NAME_LENGTH];
            scanf("%s", competitionName);
            if (strcmp(competitionName, "q") == 0) {
                break;
            }
            while (current != NULL) {
                if (strcmp(current->competitionName, competitionName) == 0) {
                    printf("找到竞赛: %s, 当前竞赛类别: %c\n", current->competitionName, current->competitionType);
                    printf("请输入新的竞赛类别: (A, B, C)");
                    char competitionType;
                    scanf("%c", &competitionType);
                    //判断输入是否合法
                    while(isCompetitionTypeValid(competitionType)){
                        handleInputError("类别应为A，B，C，请重新输入：");
                        scanf("%c", &competitionType);
                    }
                    current->competitionType = competitionType;
                    printf("竞赛类别修改成功。\n");
                    break;
                }
                current = current->next;
            }
            if (current == NULL) {
                printf("未找到指定的竞赛。\n");
            }
            break;
        }
    }
}

//删除大学生创新创业项目
void deleteInnovationProject(StudentInfo *student) {
    // 删除大学生创新创业计划项目
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    if (student->innovationProjects == NULL) {
        printf("当前没有项目可以删除。\n");
    }
    InnovationProject *current = student->innovationProjects;
    InnovationProject *prev = NULL;
    printf("请输入要删除的项目名称（输入q退出）：");
    char projectName[MAX_NAME_LENGTH];
    scanf("%s", projectName);
    if (strcmp(projectName, "q") == 0) {
        return;
    }
    if (current != NULL && strcmp(current->projectName, projectName) == 0) {
        // 要删除的是头节点
        student->innovationProjects = NULL;
        free(current); // 释放被删除节点的内存
        calculateQualityGPA(student);  //重新计算素质加分GPA
        calculateGPA(student);  //重新计算GPA
        printf("项目删除成功。\n");
        return;
    }
    if (current == NULL) {
        printf("未找到指定的项目。\n");
    }
}

//删除学术论文
void deleteAcademicPaper(StudentInfo *student) {
    // 删除学术论文
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    if (student->academicPapers == NULL) {
        printf("当前没有论文可以删除。\n");
    }
    AcademicPaper *current = student->academicPapers;
    AcademicPaper *prev = NULL;
    printf("请输入要删除的论文标题（输入q退出）：");
    char title[MAX_NAME_LENGTH];
    scanf("%s", title);
    if (strcmp(title, "q") == 0) {
        return;
    }
    while (current != NULL) {
        if (strcmp(current->title, title) == 0) {
            if (prev == NULL) {
                // 要删除的是头节点
                student->academicPapers = current->next;
            } else {
                // 要删除的是中间或尾节点
                prev->next = current->next;
            }
            AcademicPaper *temp = current;
            current = current->next;
            free(temp); // 释放被删除节点的内存
            calculateQualityGPA(student);  //重新计算素质加分GPA
            calculateGPA(student);  //重新计算GPA
            printf("论文删除成功。\n");
            break;
        }
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("未找到指定的论文。\n");
    }
}

//删除计算机类学科竞赛
void deleteCompetition(StudentInfo *student) {
    // 删除计算机类学科竞赛
    if (student == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    if (student->competitions == NULL) {
        printf("当前没有竞赛可以删除。\n");
    }
    Competition *current = student->competitions;
    Competition *prev = NULL;
    printf("请输入要删除的竞赛名称（输入q退出）：");
    char competitionName[MAX_NAME_LENGTH];
    scanf("%s", competitionName);
    if (strcmp(competitionName, "q") == 0) {
        return;
    }
    while (current != NULL) {
        if (strcmp(current->competitionName, competitionName) == 0) {
            if (prev == NULL) {
                // 要删除的是头节点
                student->competitions = current->next;
            } else {
                // 要删除的是中间或尾节点
                prev->next = current->next;
            }
            Competition *temp = current;
            current = current->next;
            free(temp); // 释放被删除节点的内存
            calculateQualityGPA(student);  //重新计算素质加分GPA
            calculateGPA(student);  //重新计算GPA
            printf("竞赛删除成功。\n");
            break;
        }
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("未找到指定的竞赛。\n");
    }
}

//按班级显示学生信息
void displayByClass(StudentInfo *head) {
    // 按班级显示学生信息
    if (head == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    int classNumber;
    int grade;
    printf("请输入年级：");
    scanf("%d", &grade);
    //判断输入是否在范围内
    while (isGradeValid(grade)) {
        handleInputError("年级必须在1-4之间,请重新输入\n");
        scanf("%d", &grade);
    }
    printf("请输入班级：");
    scanf("%d", &classNumber);
    //判断输入是否在范围内
    while (isClassValid(classNumber)) {
        handleInputError("班级必须在为大于0的整数,请重新输入\n");
        scanf("%d", &classNumber);
    }
    StudentInfo *current = head;
    while (current != NULL) {
        if (current->classNumber == classNumber && current->grade == grade) {
            printf("学号: %s, 姓名: %s, 班级: %d\n学业GPA: %f, 已获总学分: %.2f, 素质项目加分GPA: %.2f, 加权平均分: %f, 综合GPA: %.2f\n\n",
                   current->studentID, current->name, current->classNumber, current->gpa, current->totalCredit,
                   current->qualityGpa, current->averageScore, current->totalGPA);
            current = current->next;
        }
    }
}

//显示学生菜单
void displayStudentMenu(){
    // 显示学生菜单
    printf("1. 显示全部学生信息\n");
    printf("2. 显示一个年级的学生信息\n");
    printf("3. 显示一个班级的学生信息\n");
    printf("5. 返回上一级菜单\n");
    int choice;
    scanf("%d", &choice);
    //判断用户选择并执行相应操作
    while (choice < 1 || choice > 5) {
        handleInputError("选项应在1-5之间，请重新输入：");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:
            displayAllStudents(studentsList);
            break;
        case 2:
            displayByGrade(studentsList);
            break;
        case 3:
            displayByClass(studentsList);
            break;
        case 5:
            return;
        default:
            handleInputError("无效的选项");
    }
}

//按年级显示学生信息
void displayByGrade(StudentInfo *head) {
    // 按年级显示学生信息
    if (head == NULL) {
        handleInputError("学生信息不能为空。\n");
        return;
    }
    int grade;
    printf("请输入年级：");
    scanf("%d", &grade);
    //判断输入是否在范围内
    while (isGradeValid(grade)) {
        handleInputError("年级必须在1-4之间,请重新输入\n");
        scanf("%d", &grade);
    }
    StudentInfo *current = head;
    while (current != NULL) {
        if (current->grade == grade) {
            printf("学号: %s, 姓名: %s, 班级: %d\n学业GPA: %f, 已获总学分: %.2f, 素质项目加分GPA: %.2f, 加权平均分: %f, 综合GPA: %.2f\n\n",
                   current->studentID, current->name, current->classNumber, current->gpa, current->totalCredit,
                   current->qualityGpa, current->averageScore, current->totalGPA);
            current = current->next;
        }
    }
}

//按年级排序
void sortByGrade(StudentInfo **head) {
    printf("请输入要排序的年级\n");
    int grade;
    scanf("%d", &grade);
    //判断输入是否在范围内
    while (isGradeValid(grade)) {
        handleInputError("年级必须在1-4之间,请重新输入\n");
        scanf("%d", &grade);
    }
    int choice;
    printf("请选择排序方式：\n");
    printf("1. 按学号排序\n");
    printf("2. 按学业GPA排序\n");
    printf("3. 按加权平均分排序\n");
    printf("4. 按照总绩点排序\n");
    printf("5. 返回\n");
    StudentInfo *tempList = NULL;
    // 创建一个临时链表用于存放按年级筛选的学生
    StudentInfo *current = *head;
    while (current != NULL) {
        if (current->grade == grade) {
            StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
            if (newStudent == NULL) {
                handleInputError("内存分配失败");
                break;
            }
            memcpy(newStudent, current, sizeof(StudentInfo));//复制学生信息
            newStudent->next = NULL;
            insertStudent(&tempList, newStudent);//插入学生信息
        }
        current = current->next;
    }
    scanf("%d", &choice);
    //判断输入是否在范围内
    while (choice < 1 || choice > 5) {
        handleInputError("输入应是1-5的数字，请重新输入\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:
            sortList(&tempList, compareByStudentID);
            break;
        case 2:
            sortList(&tempList, compareByGPA);
            break;
        case 3:
            sortList(&tempList, compareByAverageScore);
            break;
        case 4:
            sortList(&tempList, compareByTotalGPA);
            displayAllStudents(tempList);
            break;
        case 5:
            return;
        default:
            handleInputError("无效的选项");
    }
    displayAllStudents(tempList);//显示排序后的学生信息
    freeMemory(&tempList);//释放临时链表的内存
}

//按班级排序
void sortByClass(StudentInfo **head){
    printf("请输入要排序的年级（1.大一 2.大二 3.大三 4.大四）\n");
    int grade;
    scanf("%d", &grade);

    //判断输入是否在范围内
    while(isGradeValid(grade)){
        handleInputError("年级必须在1-4之间,请重新输入\n");
        scanf("%d", &grade);
    }

    printf("请输入要排序的班级（一个正整数）\n");
    int classNumber;
    scanf("%d", &classNumber);
    //判断输入是否是正整数
    while (isClassValid(classNumber)) {
        handleInputError("班级必须是正整数,请重新输入\n");
        scanf("%d", &classNumber);
    }
    int choice;
    printf("请选择排序方式：\n");
    printf("1. 按学号排序\n");
    printf("2. 按学业GPA排序\n");
    printf("3. 按加权平均分排序\n");
    printf("4. 按照总绩点排序\n");
    printf("5. 返回\n");

    StudentInfo *tempList = NULL;
    // 创建一个临时链表用于存放按年级筛选的学生
    StudentInfo *current = *head;
    while (current != NULL) {
        if (current->classNumber == classNumber) {
            StudentInfo *newStudent = (StudentInfo *)malloc(sizeof(StudentInfo));
            if (newStudent == NULL) {
                handleInputError("内存分配失败");
                break;
            }
            memcpy(newStudent, current, sizeof(StudentInfo));//复制学生信息
            newStudent->next = NULL;
            insertStudent(&tempList, newStudent);//插入学生信息
        }
        current = current->next;
    }
    scanf("%d", &choice);
    //判断输入是否在范围内
    while (choice < 1 || choice > 5) {
        handleInputError("输入应是1-5的数字，请重新输入\n");
        scanf("%d", &choice);
    }
    switch (choice) {
        case 1:
            sortList(&tempList, compareByStudentID);
            break;
        case 2:
            sortList(&tempList, compareByGPA);
            break;
        case 3:
            sortList(&tempList, compareByAverageScore);
            break;
        case 4:
            sortList(&tempList, compareByTotalGPA);
            displayAllStudents(tempList);
            break;
        case 5:
            return;
        default:
            handleInputError("无效的选项");
    }
    freeMemory(&tempList);//释放临时链表的内存
}

//判断学号是否合法
int isStudentIDValid(const char *studentID) {
    // 判断学号是否合法
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

//判断字符是否是数字
int isDigit(char c) {
    return c >= '0' && c <= '9';
}

//判断年级是否合法
int isGradeValid(int grade) {
    return grade >= 1 && grade <= 4;
}

//判断班级是否合法
int isClassValid(int classNumber) {
    // 判断班级是否合法
    return classNumber > 0;
}

// 判断分数是否合法
int isScoreValid(float score) {
    return score >= 0 && score <= 100;
}

// 判断学分是否合法
int isCreditValid(float credit) {
    //学分应大于0小于MAX_CREDIT且为0.5的整数倍
    // 检查学分是否大于0
    if (credit <= 0) {
        return 0; // 不合法，学分应大于0
    }
    // 检查学分是否小于等于最大学分
    if (credit > MAX_CREDIT) {
        return 0; // 不合法，学分超过最大值
    }
    // 检查学分是否为0.5的整数倍
    if (credit != (int)(credit / 0.5) * 0.5) {
        return 0; // 不合法，学分不是0.5的整数倍
    }
    // 如果所有条件都满足，则学分合法
    return 1;
}

// 判断密码是否合法
int isPasswordValid(const char *password) {
    if (strlen(password) < 1 || strlen(password) > MAX_PASSWORD_LENGTH) {
        return 0;
    }
    return 1;
}

//判断大创项目级别是否合法
int isProjectLevelValid(const char *projectLevel){
    if(strcmp(projectLevel, "国家级") != 0 && strcmp(projectLevel, "省级") != 0 && strcmp(projectLevel, "校级") != 0){
        return 1;
    }
    return 0;
}

//判断期刊级别是否合法
int isJournalLevelValid(int journalLevel){
    if(journalLevel < 1 || journalLevel > 6){
        return 1;
    }
    return 0;
}

//判断竞赛级别是否合法
int isCompetitionLevelValid(const char *competitionLevel){
    if(strcmp(competitionLevel, "国家级") != 0 && strcmp(competitionLevel, "省级") != 0 && strcmp(competitionLevel, "校级") != 0){
        return 1;
    }
    return 0;
}

//判断竞赛类别是否合法
int isCompetitionTypeValid(char competitionType){
    if(competitionType != 'A' && competitionType != 'B' && competitionType != 'C'){
        return 1;
    }
    return 0;
}

//判断获奖等级是否合法
int isRankValid(int rank){
    if(rank != 1 && rank != 2 && rank != 3){
        return 1;
    }
    return 0;
}

//判断是否为负责人是否合法
int isLeaderOrSecondLeaderValid(int isLeaderOrSecondLeader){
    if(isLeaderOrSecondLeader != 0 && isLeaderOrSecondLeader != 1 && isLeaderOrSecondLeader != 2){
        return 1;
    }
    return 0;
}