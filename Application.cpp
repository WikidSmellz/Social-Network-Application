#include<iostream>
#include<fstream>
#include<cstring>
#include <ctime>  // For time functions 

using namespace std;

class Controller;
class Obj {
protected:
    char* ID;
public:
    Obj();
    virtual ~Obj();
    virtual const char* GetID() const = 0;
    virtual void Print() const = 0;
    virtual void PrintShort() const = 0;
    virtual bool IsUser() const { return false; }
    virtual bool IsPage() const { return false; }
};

class Date {
    int day;
    int month;
    int year;
public:
    Date(int d = 0, int m = 0, int y = 0);
    static Date GetCurrentDate();
    void Print() const;
    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;
    void SetDay(int d);
    void SetMonth(int m);
    void SetYear(int y);
    int DaysDifference(const Date& other) const;
};

class Comment {
    char* ID;
    char* PostID;
    char* UserID;
    char* Text;
public:
    Comment();
    ~Comment();
    void LoadDataFromFile(ifstream& file);
    void Print(const Controller& controller) const;  
    void CreateComment(const char* postId, const char* userId, const char* text);
    friend class Controller;
};

class Post : public Obj {

    Comment** AllComments;
    Obj* SharedBy;
    Date SharedDate;
    char* Text;
    Obj** LikedBy;
    int totallikes;
    const int maxlikes;
    int totalcomments;
    friend class Controller;
    Date originalPostDate; // in the sample this is 3 years ago 

public:
    Post();
    ~Post();
    const char* GetID() const override;
    const char* GetText() const;
    Obj* GetSharedBy() const;
    Date GetDate() const;
    void Print() const override;
    void PrintShort() const override;
    void PrintTimeline() const;
    void ViewLikedList() const;
    void LoadDataFromFile(ifstream& fin, Obj** allUsers, Obj** allPages, int userCount, int pageCount);
    void LoadCommentsFromFile(ifstream& file);
    int GetTotalComments() const;
    void ViewComments(const Controller& cont) const;  
    void AddComment(Comment* comment);

  
};

class Page : public Obj {
private:
    char* Title;
public:
    Page();
    ~Page();
    const char* GetID() const override;
    const char* GetTitle() const;
    void Print() const override;
    void PrintShort() const override;
    void LoadDataFromFile(ifstream& fin);
    
    bool IsUser() const override { return false; }
    bool IsPage() const override { return true; }
};

class User : public Obj {
private:
    char* fName;
    char* lName;
    User** Friends;
    Page** LikedPages;
    Post** TimelinePosts;
    int totalfriends;
    int total_liked;
    int total_posts;
    const int max_friends;
    const int max_liked;
    const int max_posts;
  
    bool IsUser() const override { return true; }
    bool IsPage() const override { return false; }

public:
   
    User();
    ~User();
    const char* GetID() const override;
    const char* GetFname() const;
    const char* GetLname() const;
    int GetTotalPosts();
    int  GetMaxPosts();
    void Print() const override;
    void PrintShort() const override;
    int GetTotalFriends() const;
    User* GetFriend(int index) const;
    void LoadDataFromFile(ifstream& fin);
    bool AddFriend(User* obj);
    bool LikePage(Page* obj);
    bool HasLikedPage(Page* page) const;
    void basicinfo() const;
    void friendlist() const;
    void Pagelist() const;
    void ViewTimeline(const Post* const allPosts[], int totalPosts) const;
    bool AddToTimeline(Post* post);


        
};

class Controller {
    User** AllUsers;
    Page** AllPages;
    Post** AllPosts;
    int total_users;
    int total_pages;
    int total_posts;

public:

   

   Controller();
    ~Controller();
    User* SearchUserByID(const char* search) const;
    Page* SearchPageByID(const char* search) const;
    Post* SearchPostByID(const char* search) const;
    void LoadUsersFromFile(ifstream& fin);
    void LoadPagesFromFile(ifstream& fin);
    void LoadPostsFromFile(ifstream& fin);
    void LinkUsersAndPages(ifstream& fin);
    void LoadData();
    void ViewLikedList(const char* postID) const;
    void ViewPage(const char* pageID) const;
    void LoadCommentsFromFile(ifstream& fin);
    void ViewHome(User* currentUser) const;
    void PostComment(const char* postID, const char* commentText, const char* userID);
    void ViewPost(const char* postID)const;
    void SeeYourMemories(const char* userID)const;
    bool AddPostToUserTimeline(const char* userID, const char* postID);
    void AddNewPost(Post* newPost);

    void ShareMemory(const char* userID, const char* postID, const char* memoryText);
    void Run();
};

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------OBJECT CLASS FUNCTION DEFINTIONS------------------------------
//----------------------------------------------------------------------------------------------------------------


Obj::Obj() {
    ID = nullptr;
}

Obj::~Obj() {
    if (ID) delete[] ID;
}
//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------DATE CLASS FUNCTION DEFINTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------------




Date::Date(int d, int m, int y) : day(d), month(m), year(y) {}

Date Date::GetCurrentDate() {
   
    return Date(17,4,2024);  // Construct and return the Date object
}

void Date::Print() const {
    cout << day << " " << month << " " << year;
}
//GETTERS &SETTERS
int Date::GetDay() const { return day; }
int Date::GetMonth() const { return month; }
int Date::GetYear() const { return year; }
void Date::SetDay(int d) { day = d; }
void Date::SetMonth(int m) { month = m; }
void Date::SetYear(int y) { year = y; }

int Date::DaysDifference(const Date& other) const {
    // this logic is used from the website mentioned in the phase 3 file
    tm a = { 0 }, b = { 0 };
    a.tm_mday = day;    a.tm_mon = month - 1;    a.tm_year = year - 1900;
    b.tm_mday = other.day; b.tm_mon = other.month - 1; b.tm_year = other.year - 1900;

    time_t t1 = mktime(&a);
    time_t t2 = mktime(&b);

    double seconds = difftime(t1, t2);
    return abs(int(seconds / (60 * 60 * 24)));  // days
}

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------PAGE CLASS FUNCTION DEFINTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------------


Page::Page() {
    Title = nullptr;
}

Page::~Page() {
    if (Title) delete[] Title;
}
//GETTERS
const char* Page::GetID() const {
    if (ID) return ID;
}

const char* Page::GetTitle() const {
    if (Title) return Title;
}

void Page::Print() const {
    if (ID) cout << "ID: " << ID << endl;
    if (Title) cout << "Title: " << Title << endl;
}

void Page::PrintShort() const {
    if (Title) cout << Title;
}

void Page::LoadDataFromFile(ifstream& fin) {
    char buffer[80];
    fin >> buffer;
    ID = new char[strlen(buffer) + 1];
    strcpy_s(ID, strlen(buffer) + 1, buffer);

    fin.get();
    fin.getline(buffer, 80);
    Title = new char[strlen(buffer) + 1];
    strcpy_s(Title, strlen(buffer) + 1, buffer);
}

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------COMMENT CLASS FUNCTION DEFINTIONS-----------------------------
//----------------------------------------------------------------------------------------------------------------



Comment::Comment() : ID(nullptr), PostID(nullptr), UserID(nullptr), Text(nullptr) {}

Comment::~Comment() {
    if (ID) delete[] ID;
    if (PostID) delete[] PostID;
    if (UserID) delete[] UserID;
    if (Text) delete[] Text;
}

void Comment::LoadDataFromFile(ifstream& file) {
    char buffer[500];

    // Read comment ID
    file >> buffer;
    ID = new char[strlen(buffer) + 1];
    strcpy_s(ID, strlen(buffer) + 1, buffer);

    // Read post ID
    file >> buffer;
    PostID = new char[strlen(buffer) + 1];
    strcpy_s(PostID, strlen(buffer) + 1, buffer);

    // Read user ID
    file >> buffer;
    UserID = new char[strlen(buffer) + 1];
    strcpy_s(UserID, strlen(buffer) + 1, buffer);

    // Read comment text
    file.ignore();
    file.getline(buffer, 500);
    Text = new char[strlen(buffer) + 1];
    strcpy_s(Text, strlen(buffer) + 1, buffer);
}

void Comment::Print(const Controller& controller) const {
    User* user = controller.SearchUserByID(UserID);
    if (user) {
        cout << "\t\t" << user->GetFname() << " " << user->GetLname();
    }
    else {
        Page* page = controller.SearchPageByID(UserID);
        if (page) {
            cout << "\t\t" << page->GetTitle();
        }
    }
    cout << ": \"" << Text << "\"\n";
}
void Comment:: CreateComment(const char* postId, const char* userId, const char* text) {
    // Generate a simple ID based on current time
    //MAAM POSTED CONTENT RELATED TO THIS ON GCR
    PostID = new char[strlen(postId) + 1];
    strcpy_s(PostID, strlen(postId) + 1, postId);

    UserID = new char[strlen(userId) + 1];
    strcpy_s(UserID, strlen(userId) + 1, userId);

    Text = new char[strlen(text) + 1];
    strcpy_s(Text, strlen(text) + 1, text);
}

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------POST CLASS FUNCTION DEFINTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------------


Post::Post() : SharedBy(nullptr), Text(nullptr), LikedBy(nullptr),
totallikes(0), maxlikes(10), totalcomments(0), AllComments(nullptr) {
    LikedBy = new Obj * [maxlikes]();
}

Post::~Post() {
    if (Text) delete[] Text;
    if (LikedBy) delete[] LikedBy;
    for (int i = 0; i < totalcomments; i++) {
        delete AllComments[i];
    }
    delete[] AllComments;
}
//GETTERS
const char* Post::GetID() const {
    if (ID) return ID;
}

const char* Post::GetText() const {
    if (Text) return Text;
}

Obj* Post::GetSharedBy() const {
    if (SharedBy) return SharedBy;
}

Date Post::GetDate() const {
    return SharedDate;
}

void Post::Print() const {
    if (ID) cout << "--- Post " << ID << " ---\n";
    cout << "Date: "; SharedDate.Print();
    if (Text) cout << "\nText: " << Text << "\nShared by: ";
    if (SharedBy) SharedBy->PrintShort();
    cout << endl;
}

void Post::PrintShort() const {
    if (Text) cout << "\"" << Text << "\"";
}

void Post::PrintTimeline() const {
   

    if (SharedBy) SharedBy->PrintShort();
    cout << " (";
    SharedDate.Print();
    cout << "):\n";
    if (Text) cout << "\"" << Text << "\"\n";
}

void Post::ViewLikedList() const {
    for (int i = 0; i < totallikes; i++) {
        if (LikedBy[i]) {
            cout << LikedBy[i]->GetID() << " - ";
            LikedBy[i]->PrintShort();
            cout << endl;
        }
    }
    cout << "----------------------------------------------------------------\n";
}

void Post::LoadDataFromFile(ifstream& fin, Obj** allUsers, Obj** allPages, int userCount, int pageCount) {
    char buffer[500];
    int day, month, year;

    fin >> buffer;
    ID = new char[strlen(buffer) + 1];
    strcpy_s(ID, strlen(buffer) + 1, buffer);

    fin >> day >> month >> year;
    SharedDate.SetDay(day);
    SharedDate.SetMonth(month);
    SharedDate.SetYear(year);
    fin.ignore(numeric_limits<streamsize>::max(), '\n');// this syntax used to ensure that ALL of the whitespaces have been ignored

    fin.getline(buffer, 500);
    Text = new char[strlen(buffer) + 1];
    strcpy_s(Text, strlen(buffer) + 1, buffer);

    fin >> buffer;
    SharedBy = nullptr;
    //CHECKING WHETHER ITS SHAREDBY USER OR PAGE
    if (buffer[0] == 'u') {
        for (int i = 0; i < userCount; i++) {
            if (allUsers[i] && strcmp(allUsers[i]->GetID(), buffer) == 0) {
                SharedBy = allUsers[i];
                break;
            }
        }
    }
    else if (buffer[0] == 'p') {
        for (int i = 0; i < pageCount; i++) {
            if (allPages[i] && strcmp(allPages[i]->GetID(), buffer) == 0) {
                SharedBy = allPages[i];
                break;
            }
        }
    }

    while (fin >> buffer && strcmp(buffer, "-1") != 0 && totallikes < maxlikes) {
        if (buffer[0] == 'u') {
            for (int i = 0; i < userCount; i++) {
                if (allUsers[i] && strcmp(allUsers[i]->GetID(), buffer) == 0) {
                    LikedBy[totallikes++] = allUsers[i];
                    break;
                }
            }
        }
        else if (buffer[0] == 'p') {
            for (int i = 0; i < pageCount; i++) {
                if (allPages[i] && strcmp(allPages[i]->GetID(), buffer) == 0) {
                    LikedBy[totallikes++] = allPages[i];
                    break;
                }
            }
        }
    }
}

void Post::LoadCommentsFromFile(ifstream& file) {
    file >> totalcomments;
    AllComments = new Comment * [totalcomments];
    for (int i = 0; i < totalcomments; i++) {
        AllComments[i] = new Comment();
        AllComments[i]->LoadDataFromFile(file);
    }
}

int Post::GetTotalComments() const { return totalcomments; }

void Post::ViewComments(const Controller& cont) const {
    cout << "Comments for "<<ID<<endl;
    for (int i = 0; i < totalcomments; i++) {
        if (AllComments[i])
            AllComments[i]->Print(cont);
    }
}

void Post:: AddComment(Comment* comment) {
    if (totalcomments >= maxlikes) return; // Check

    // Create new array with +1 size
    // extra size for the new comment
    Comment** newComments = new Comment * [totalcomments + 1];

    // Copy old comments
    for (int i = 0; i < totalcomments; i++) {
        newComments[i] = AllComments[i];
    }

    // Add new comment
    newComments[totalcomments] = comment;

    // Replace old array
    delete[] AllComments;
    AllComments = newComments;
    totalcomments++;
}
//----------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------CONTROLLER CLASS FUNCTION DEFINTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------------------


Controller::Controller() : AllUsers(nullptr), AllPages(nullptr), AllPosts(nullptr),
total_users(0), total_pages(0), total_posts(0) {}

Controller::~Controller() {
    for (int i = 0; i < total_users; i++) delete AllUsers[i];
    delete[] AllUsers;

    for (int i = 0; i < total_pages; i++) delete AllPages[i];
    delete[] AllPages;

    for (int i = 0; i < total_posts; i++) delete AllPosts[i];
    delete[] AllPosts;
}

User* Controller::SearchUserByID(const char* search) const {
    if (!search) return nullptr;
    for (int i = 0; i < total_users; i++) {
        if (AllUsers[i] && strcmp(search, AllUsers[i]->GetID()) == 0)
            return AllUsers[i];
    }
    return nullptr;
}

Page* Controller::SearchPageByID(const char* search) const {
    if (!search) return nullptr;
    for (int i = 0; i < total_pages; i++) {
        if (AllPages[i] && strcmp(search, AllPages[i]->GetID()) == 0)
            return AllPages[i];
    }
    return nullptr;
}

Post* Controller::SearchPostByID(const char* search) const {
    if (!search) return nullptr;
    for (int i = 0; i < total_posts; i++) {
        if (AllPosts[i] && strcmp(search, AllPosts[i]->GetID()) == 0)
            return AllPosts[i];
    }
    return nullptr;
}

void Controller::LoadUsersFromFile(ifstream& fin) {
    fin >> total_users;
    AllUsers = new User * [total_users]();
    for (int i = 0; i < total_users; i++) {
        AllUsers[i] = new User();
        AllUsers[i]->LoadDataFromFile(fin);
    }
}

void Controller::LoadPagesFromFile(ifstream& fin) {
    fin >> total_pages;
    AllPages = new Page * [total_pages]();
    for (int i = 0; i < total_pages; i++) {
        AllPages[i] = new Page();
        AllPages[i]->LoadDataFromFile(fin);
    }
}

void Controller::LoadPostsFromFile(ifstream& fin) {
    fin >> total_posts;
    AllPosts = new Post * [total_posts]();
    for (int i = 0; i < total_posts; i++) {
        AllPosts[i] = new Post();
        AllPosts[i]->LoadDataFromFile(fin, (Obj**)AllUsers, (Obj**)AllPages,
            total_users, total_pages);
    }
}

void Controller::LinkUsersAndPages(ifstream& fin) {
    char buffer[20];
    while (fin >> buffer) {
        User* temp = SearchUserByID(buffer);
        if (!temp) continue;

        while (fin >> buffer && strcmp(buffer, "-1") != 0) {
            User* friendPtr = SearchUserByID(buffer);
            if (friendPtr) temp->AddFriend(friendPtr);
        }

        while (fin >> buffer && strcmp(buffer, "-1") != 0) {
            Page* likedPage = SearchPageByID(buffer);
            if (likedPage) temp->LikePage(likedPage);
        }
    }
}

void Controller::LoadData() {
    //CALL TO EACH TXT FILE FOR LOADING DATA
    ifstream file("SocialNetworkUsers.txt");
    if (file) LoadUsersFromFile(file);
    else cerr << "Error opening users file\n";
    file.close();

    ifstream file1("SocialNetworkPages.txt");
    if (file1) LoadPagesFromFile(file1);
    else cerr << "Error opening pages file\n";
    file1.close();

    ifstream file2("UsersFriendsAndLikedPages.txt");
    if (file2) LinkUsersAndPages(file2);
    else cerr << "Error opening friends/liked pages file\n";
    file2.close();

    ifstream file3("SocialNetworkPosts.txt");
    if (file3) LoadPostsFromFile(file3);
    else cerr << "Error opening posts file\n";
    file3.close();
    ifstream file4("SocialNetworkComments.txt");
    if (file4) LoadCommentsFromFile(file4);
    else cerr << "error opening comment file\n";
    file4.close();
}

void Controller::ViewLikedList(const char* postID) const {
    cout << "--------------------------------------------------\n";
    cout << "Command:\t\tViewLikedList(" << postID << ")" << endl;
    if (!postID) {
        cerr << "Error: Null post ID provided\n";
        return;
    }
    Post* post = SearchPostByID(postID);
    if (post) post->ViewLikedList();
    else cerr << "Post not found: " << postID << endl;
}

void Controller::ViewPage(const char* pageID) const {
    cout << "Command:\t\tViewPage(" << pageID << ")" << endl;
    if (!pageID) {
        cerr << "Error: Null page ID provided\n";
        return;
    }
    Page* page = SearchPageByID(pageID);
    if (page) {
        for (int i = 0; i < total_posts; i++) {
            if (AllPosts[i] && AllPosts[i]->GetSharedBy() == page) {
                AllPosts[i]->PrintTimeline();
                cout << endl;
            }
        }
        cout << "--------------------------------------------------\n";
    }
    else {
        cerr << "Page not found: " << pageID << endl;
    }
}

void Controller::LoadCommentsFromFile(ifstream& fin) {
    char commentID[50], postID[50], userID[50], text[500];//these are buffers

    while (fin >> commentID >> postID >> userID) {
        fin.ignore();  // Skip the space before the comment text
        fin.getline(text, 500);

        Post* post = SearchPostByID(postID);
        if (post) {
            // Allocate memory for new comment
            Comment* newComment = new Comment();

            // Manually set comment fields (bypassing LoadDataFromFile)
            newComment->ID = new char[strlen(commentID) + 1];
            strcpy_s(newComment->ID, strlen(commentID) + 1, commentID);

            newComment->PostID = new char[strlen(postID) + 1];
            strcpy_s(newComment->PostID, strlen(postID) + 1, postID);

            newComment->UserID = new char[strlen(userID) + 1];
            strcpy_s(newComment->UserID, strlen(userID) + 1, userID);

            newComment->Text = new char[strlen(text) + 1];
            strcpy_s(newComment->Text, strlen(text) + 1, text);

            // Add to post's comments
            Comment** updatedList = new Comment * [post->GetTotalComments() + 1];
            for (int i = 0; i < post->GetTotalComments(); i++) {
                updatedList[i] = post->AllComments[i];  // Copy old
            }
            updatedList[post->GetTotalComments()] = newComment;  // Add new

            // Delete old list and replace
            delete[] post->AllComments;
            post->AllComments = updatedList;
            post->totalcomments++;
        }
    }
}

void Controller::ViewHome(User* currentUser) const {
    if (!currentUser) {//CHECK
        cout << "Error: No current user set!\n";
        return;
    }

    cout << "Command:\t\tView Home\n";
    cout << "----------------------------------------------------------------\n";
    cout << currentUser->GetFname() << " " << currentUser->GetLname() << " - Home Page\n\n";

    Date currentDate = Date::GetCurrentDate();

    // First pass: count how many posts we need to display
    int visibleCount = 0;
    for (int i = 0; i < total_posts; i++) {
        Post* post = AllPosts[i];
        if (!post) continue;

        Obj* sharedBy = post->GetSharedBy();
        if (!sharedBy) continue;

        int diff = currentDate.DaysDifference(post->GetDate());
        if (diff > 3) continue;// IF DIFF ISMORE THAN 3 YEARSS

        if (sharedBy == currentUser) {
            visibleCount++;
        }
        else if (sharedBy->IsUser()) {
            User* sharedUser = static_cast<User*>(sharedBy);
            for (int j = 0; j < currentUser->GetTotalFriends(); j++) {
                if (currentUser->GetFriend(j) == sharedUser) {// if post already shared by friend(user) count++
                    visibleCount++;
                    break;
                }
            }
        }
        else if (sharedBy->IsPage()) {
            Page* sharedPage = static_cast<Page*>(sharedBy);
            if (currentUser->HasLikedPage(sharedPage)) {//ifpost is liked already (meaning it exists) count++
                visibleCount++;
            }
        }
    }

    if (visibleCount == 0) {
        cout << "No recent posts to display (within last 3 days).\n";
        cout << "----------------------------------------------------------------\n";
        return;
    }

    // Create array to store visible posts
    Post** visiblePosts = new Post * [visibleCount];
    int* daysDiff = new int[visibleCount];
    int index = 0;

    // Second pass: collect visible posts
    for (int i = 0; i < total_posts; i++) {
        Post* post = AllPosts[i];
        if (!post) continue;

        Obj* sharedBy = post->GetSharedBy();
        if (!sharedBy) continue;

        int diff = currentDate.DaysDifference(post->GetDate());
        if (diff > 3) continue;

        bool shouldDisplay = false;

        if (sharedBy == currentUser) {
            shouldDisplay = true;
        }
        else if (sharedBy->IsUser()) {
            User* sharedUser = static_cast<User*>(sharedBy);
            for (int j = 0; j < currentUser->GetTotalFriends(); j++) {
                if (currentUser->GetFriend(j) == sharedUser) {
                    shouldDisplay = true;
                    break;
                }
            }
        }
        else if (sharedBy->IsPage()) {
            Page* sharedPage = static_cast<Page*>(sharedBy);
            if (currentUser->HasLikedPage(sharedPage)) {
                shouldDisplay = true;
            }
        }

        if (shouldDisplay) {
            visiblePosts[index] = post;
            daysDiff[index] = diff;
            index++;
        }
    }

    // Display the posts
    for (int i = 0; i < index; i++) {
        Post* post = visiblePosts[i];
        int diff = daysDiff[i];
        Obj* sharedBy = post->GetSharedBy();

        cout << "--- ";
        sharedBy->PrintShort();

        if (diff == 0) {
            cout << " (today):\n";
        }
        else if (diff == 1) {
            cout << " (yesterday):\n";
        }
        else {
            cout << " (" << diff << " days ago):\n";
        }

        cout << "\t\"" << post->GetText() << "\"\n";

        if (post->GetTotalComments() > 0) {
            //cout << "\tComments:\n";
            post->ViewComments(*this);
        }
        cout << endl;
    }

    // Clean up
    delete[] visiblePosts;
    delete[] daysDiff;
    cout << "----------------------------------------------------------------\n";
}
void Controller:: PostComment(const char* postID, const char* commentText, const char* userID) {
    cout << "Command:\t\tPostComment(" << postID << ", \"" << commentText << "\")\n";

    // Find the post
    Post* post = SearchPostByID(postID);
    if (!post) {
        cout << "Error: Post not found\n";
        return;
    }

    // Find the user
    User* user = SearchUserByID(userID);
    if (!user) {
        cout << "Error: User not found\n";
        return;
    }

    // Create new comment
    Comment* newComment = new Comment();
    newComment->CreateComment(postID, userID, commentText);

    // Add to post
    post->AddComment(newComment);

    cout << "Comment added successfully!\n";
    cout << "--------------------------------------------------\n";
}

void Controller::ViewPost(const char* postID) const {
    cout << "--------------------------------------------------\n";
    cout << "Command:\t\tViewPost(" << postID << ")\n";

    Post* post = SearchPostByID(postID);
    if (!post) {
        cout << "Error: Post not found\n";
        return;
    }

    // Print post details
    cout << "--- Post Details ---\n";
    post->PrintTimeline();
    cout << "\n";

    // Print likes count and list
    cout << "Likes (" << post->totallikes << "):\n";
    post->ViewLikedList();

    // Print comments
    cout << "Comments (" << post->GetTotalComments() << "):\n";
    post->ViewComments(*this);

    cout << "--------------------------------------------------\n";
}

void Controller::Run() {
    User* user = SearchUserByID("u7");
    if (user) {
        cout << "\t-----WELCOME TO THE SOCIAL MEDIA APPLICATION----\n";
        user->basicinfo();
        user->friendlist();
        user->Pagelist();
        user->ViewTimeline((const Post**)AllPosts, total_posts);
        ViewLikedList("post5");
        ViewPage("p1");
        ViewHome(user);
        SeeYourMemories("u7");
       
        ShareMemory("u7", "post10", "Never thought I would specialize in this field");
        user->ViewTimeline((const Post**)AllPosts, total_posts);
    }
}
bool Controller:: AddPostToUserTimeline(const char* userID, const char* postID) {
    User* user = SearchUserByID(userID);
    Post* post = SearchPostByID(postID);

    if (!user || !post) {
        return false;
    }


    return user->AddToTimeline(post);
}
void Controller::AddNewPost(Post* newPost) {
    Post** newPosts = new Post * [total_posts + 1];
    for (int i = 0; i < total_posts; i++) {
        newPosts[i] = AllPosts[i];
    }
    newPosts[total_posts] = newPost;
    delete[] AllPosts;
    AllPosts = newPosts;
    total_posts++;
}
void Controller:: ShareMemory(const char* userID, const char* postID, const char* memoryText) {
    cout << "Command:\t\tShareMemory(" << postID << ", \"" << memoryText << "\")\n";

    User* user = SearchUserByID(userID);
    if (!user) {
        cout << "Error: User not found\n";
        return;
    }

    Post* originalPost = SearchPostByID(postID);
    if (!originalPost) {
        cout << "Error: Original post not found\n";
        return;
    }

    // Calculate years difference
    Date currentDate = Date::GetCurrentDate();
    int yearsDiff = currentDate.GetYear() - originalPost->GetDate().GetYear();

    // Create memory post content
    string memoryContent = "~~~ ";
    memoryContent += user->GetFname();
    memoryContent += " ";
    memoryContent += user->GetLname();
    memoryContent += " shared a memory ~~~\n\"";
    memoryContent += memoryText;
    memoryContent += "\"\n~~~ ";


    // Create new memory post
    Post* memoryPost = new Post();

    // Set memory post ID (original post ID + "_mem")
    //USED STRING DATATYPE TO CONCATENATE THE CONTENT OF POST
    string memoryID = string(postID) + "_mem";
    memoryPost->ID = new char[memoryID.length() + 1];
    strcpy_s(memoryPost->ID, memoryID.length() + 1, memoryID.c_str());

    // Set current date
    memoryPost->SharedDate = currentDate;

    // Set memory text
    memoryPost->Text = new char[memoryContent.length() + 1];
    strcpy_s(memoryPost->Text, memoryContent.length() + 1, memoryContent.c_str());

    // Set the sharing user as author
    memoryPost->SharedBy = user;

    Post** newPosts = new Post * [total_posts + 1];
    for (int i = 0; i < total_posts; i++) {
        newPosts[i] = AllPosts[i];
    }
    newPosts[total_posts] = memoryPost;
    delete[] AllPosts;
    AllPosts = newPosts;
    total_posts++;

    // Add to user's timeline
    if (!user->AddToTimeline(memoryPost)) {
        cout << "Warning: Memory was created but couldn't be added to user's timeline\n";
    }

}
void Controller::SeeYourMemories(const char* userID) const {
    cout << "--------------------------------------------------\n";
    cout << "Command:\t\tSeeYourMemories(" << userID << ")\n";

    User* user = SearchUserByID(userID);
    if (!user) {
        cout << "Error: User not found\n";
        return;
    }

    Date currentDate = Date::GetCurrentDate();
    bool foundMemories = false;

    cout << "===== MEMORIES FOR ";
    user->Print();
    cout << " =====\n\n";

    // Check all posts in the system
    for (int i = 0; i < total_posts; i++) {
        Post* post = AllPosts[i];
        if (!post) continue;

        // Check if this post was shared by the user
        if (post->GetSharedBy() == user) {
            int yearsDiff = currentDate.GetYear() - post->GetDate().GetYear();

            // Only show posts that are at least 1 year old
            //YOU CAN CHANGE THIS 1 TO ANY NUMBER SUITABLE
            if (yearsDiff >= 1) {
                foundMemories = true;
                cout << yearsDiff << " YEAR" << (yearsDiff > 1 ? "S" : "") << " AGO:\n";
                post->PrintTimeline();

                // Show comments if any exist
                if (post->GetTotalComments() > 0) {
                    post->ViewComments(*this);
                }
                cout << "------------------------\n";
            }
        }
    }

    if (!foundMemories) {
        cout << "No memories found (no posts older than 1 year)\n";
    }
    cout << "--------------------------------------------------\n";
}

//----------------------------------------------------------------------------------------------------------------
//--------------------------------------------------USER CLASS FUNCTION DEFINTIONS--------------------------------
//----------------------------------------------------------------------------------------------------------------


User::User() : fName(nullptr), lName(nullptr),
Friends(nullptr), LikedPages(nullptr), TimelinePosts(nullptr),
totalfriends(0), total_liked(0), total_posts(0),
max_friends(10), max_liked(10), max_posts(10) {
    Friends = new User * [max_friends]();
    LikedPages = new Page * [max_liked]();
    TimelinePosts = new Post * [max_posts]();
}

User::~User() {
    if (fName) delete[] fName;
    if (lName) delete[] lName;
    if (Friends) delete[] Friends;
    if (LikedPages) delete[] LikedPages;
    if (TimelinePosts) delete[] TimelinePosts;
}
//GETTERS
const char* User::GetID() const {
    if (ID) return ID;
}

const char* User::GetFname() const {
    if (fName) return fName;
}

const char* User::GetLname() const {
    if (lName) return lName;
}
int User::GetTotalPosts()
{
    return total_posts;
}
int User::GetMaxPosts()
{
    return max_posts;
}
void User::Print() const {
    if (fName) cout << fName;
    if (lName) cout << " " << lName;
}

void User::PrintShort() const {
    if (fName) cout << fName;
}

int User::GetTotalFriends() const { return totalfriends; }

User* User::GetFriend(int index) const {
    if (index >= 0 && index < totalfriends)
        return Friends[index];
    return nullptr;
}

void User::LoadDataFromFile(ifstream& fin) {
    char buffer[80];
    fin >> buffer;
    ID = new char[strlen(buffer) + 1];
    strcpy_s(ID, strlen(buffer) + 1, buffer);

    fin >> buffer;
    fName = new char[strlen(buffer) + 1];
    strcpy_s(fName, strlen(buffer) + 1, buffer);

    fin >> buffer;
    lName = new char[strlen(buffer) + 1];
    strcpy_s(lName, strlen(buffer) + 1, buffer);
}

bool User::AddFriend(User* obj) {
    if (!obj || totalfriends >= max_friends) return false;
    Friends[totalfriends++] = obj;
    return true;
}

bool User::LikePage(Page* obj) {
    if (!obj || total_liked >= max_liked) return false;
    LikedPages[total_liked++] = obj;
    return true;
}

bool User::HasLikedPage(Page* page) const {
    if (!page) return false;
    for (int i = 0; i < total_liked; i++) {
        if (LikedPages[i] == page) {
            return true;
        }
    }
    return false;
}

void User::basicinfo() const {
    cout << "Set Current User as: " << ID << endl;
    cout << fName << " " << lName;
    cout << " successfully set as Current User\n";
}

void User::friendlist() const {
    cout << "Command: View friend List\n";
    cout << "------------------------\n";
    for (int i = 0; i < totalfriends; i++) {
        if (Friends[i]) {
            cout << Friends[i]->GetID() << " - "
                << Friends[i]->GetFname() << " "
                << Friends[i]->GetLname() << endl;
        }
    }
}

void User::Pagelist() const {
    cout << "Command: View Liked Pages\n";
    cout << "-------------------------\n";
    for (int i = 0; i < total_liked; i++) {
        if (LikedPages[i]) {
            cout << LikedPages[i]->GetID() << " - "
                << LikedPages[i]->GetTitle() << endl;
        }
    }
}

void User::ViewTimeline(const Post* const allPosts[], int totalPosts) const {
    cout << "--------------------------------------------------\n";
    cout << fName << " " << lName;
    cout << " - Time Line\n\n";
    for (int i = 0; i < totalPosts; i++) {
        if (allPosts[i]->GetSharedBy() == this) {
            allPosts[i]->PrintTimeline();
            cout << endl;
        }
    }
}
bool User:: AddToTimeline(Post* post) {
    if (!post || total_posts >= max_posts) {//TWO CHECKS IN ONE
        return false;
    }

    // Check if post already exists in timeline
    for (int i = 0; i < total_posts; i++) {
        if (TimelinePosts[i] && strcmp(TimelinePosts[i]->GetID(), post->GetID()) == 0) {//CHECKING IF ID EXISTS OF TIMELINEPOST AND POST WHICH HAVE BEEN LOADED FROM TXT FILE
            return false; // Post already in timeline
        }
    }

    // Add to timeline
    TimelinePosts[total_posts] = post;
    total_posts++;
    return true;
}


void main() { 
    Controller myapp;
    myapp.LoadData();
    myapp.Run();
}
