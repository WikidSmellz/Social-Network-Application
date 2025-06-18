## Features
- **User Management**: Create and manage user profiles with friends lists
- **Page System**: Users can like and follow pages
- **Posting System**: Users and pages can create posts with text content
- **Commenting**: Users can comment on posts
- **Timeline**: View posts in chronological order
- **Memory Sharing**: Share memories from past posts
- **Likes System**: Track which users/pages have liked posts
- **Date Handling**: Track when posts were made and calculate time differences

## Class Structure
The application is built around several key classes:

### Main Classes
1. **Controller**: Main application controller that manages all data and operations
2. **User**: Represents a user profile with friends, liked pages, and timeline posts
3. **Page**: Represents a page that users can follow
4. **Post**: Represents a social media post with comments and likes
5. **Comment**: Represents a comment on a post
6. **Date**: Handles date calculations and formatting
7. **Obj**: Abstract base class for User and Page

## Data Files
The application loads data from several text files:
- `SocialNetworkUsers.txt`: User profile data
- `SocialNetworkPages.txt`: Page data
- `UsersFriendsAndLikedPages.txt`: Friendship and page likes relationships
- `SocialNetworkPosts.txt`: Post data
- `SocialNetworkComments.txt`: Comment data

## How to Run
1. Compile the program with a C++ compiler that supports C++11 or later
2. Ensure all data files are in the same directory as the executable
3. Run the program

## Key Functionality
- View user profiles and friend lists
- View pages and their posts
- View posts with comments and likes
- View home feed showing recent posts from friends and liked pages
- Share memories from old posts
- Post new comments

## Example Usage
The `main()` function demonstrates sample usage by:
1. Loading all data
2. Setting a current user (u7)
3. Displaying user info, friends list, and liked pages
4. Showing the user's timeline
5. Viewing likes on a post
6. Viewing a page
7. Showing the home feed
8. Viewing memories
9. Sharing a memory

## Limitations
- No input validation for file data
- Limited error handling
- No persistence of new posts/comments beyond program runtime
- Hardcoded maximums for friends, liked pages, etc.

## Future Enhancements
- Add user input for interactive operation
- Implement data persistence for new content
- Add more robust error handling
- Support for multimedia posts
- Notifications system
