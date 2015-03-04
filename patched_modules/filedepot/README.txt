
The Filedepot Document Management module satisfies the need for a full featured document management module supporting role or user based security.
 - Documents are saved to a directory under the Drupal Private file system to protect corporate documents for safe access and distribution.
 - Flexible permission model allows you to delegate folder administration to other users.
 - Define view, download and upload access for selected users, roles or groups (If OG module installed) - any combination of permissions can be setup per folder.
 - Organic Group support and options to auto-create a new top level folder for each new group.
   If you access the main filedepot link with the group id or group name passed in as an argument,
   the user will only see the folders and files under the OG created Top Level folder
 - Cloud Tag and File tagging support to organize and search your files.
   Document tagging allows users to search the repository by popular tags.
   Users can easily see what tags have been used and select one of multiple tags to display a filtered view of files in the document repository.
 - Users can upload new versions of files and the newer file will automatically be versioned. Previous versions are still available.
 - Users can selectively receive notification of new files being added or changed.
   Subscribe to individual file updates or complete folders. File owner can use the Broadcast feature to send out a personalized notification.
 - Convenient reports to view latest files, most recent folders, bookmarked files, locked or un-read files.
 - Users can flag document as 'locked' to alert users that it is being updated.
 - File Downloads are logged to the Druapl Recent Log messages
 - Check the Drupal Recent Log messages for any module errors and other module events - type: filedepot

The Filedepot module is developed by Nextide www.nextide.ca


Dependencies
------------
 * Ctools
 * libraries

 Organic Groups is not required but if you install the modules 'og' and 'og_access',
 then you will be able to manage folder access via organic groups.
 The module will automatically detect if both modules are enabled and will only
 show the group options for permissions once both 'og' and 'og_access' modules are enabled.
 Review the module configuration settings once OG is enabled to enable OG Mode settings

Requirements
------------
 PHP 5.2+ and PHP JSON library enabled.

 As of PHP 5.2.0, the JSON extension is bundled and compiled into PHP by default.


Install
-------

1) Copy the filedepot folder to the modules folder in your installation.

2) We are not permitted to include NON-GPL or mixed license files in the module distribution as per Drupal guidelines.

   Filedepot requires two external javascript libraries to be installed and uses the Drupal libraries module API to manage access to them.

   The filedepot module requires the libraries module be installed and works with version 1 or version 2 of the libraries module.
   You will need to create a sites/all/libraries folder if you don't already have the libraries module installed.
   If you have drush installed, you will want to use the provided drush command to download and install the 3rd party libraries

   MANUAL METHOD
   *** PLEASE read carefully and create the required directories (folders) and rename the files as noted below ***

   i) You need to create separate folders under sites/all/libraries for each new library with the folder name the same as the library file.
   ii) Folder names and filenames are all in lowercase
   iii) Assuming your libraries folder exists at sites/all/libraries, create two directories below the sites/all/libraries folder:
         - sites/all/libraries/html_encoder
         - sites/all/libraries/jquery.blockui
   iv) The following javascript files then need to be retrieved and saved to their respective folders under the sites/all/libraries folder.
        > http://www.strictly-software.com/scripts/downloads/encoder.js  - SAVE FILE as: html_encoder/html_encoder.js
        > http://jquery.malsup.com/block/#download  - SAVE FILE as jquery.blockui/jquery.blockui.js

   AUTOMATIC METHOD - using drush
   - Use the drush filedepot-libraries command to download and install the two required 3rd party javascript libraries.
   - If you enable the module using drush, it will automatically run the drush command to install the 3rd party libraries.

3) Check that your site has the Private file system path setup.
   Filedepot uses the private file system for it's file repository and is required.
   Typically the private file system is located outside of the website's public_html directory
   and this provides for a far more secure file repository since the files can not be
   accessed directly by a URL and there is no need to use other filesystem security like .htaccess.

4) Enable the module using admin/modules
   The module will create a new content type called 'filedepot_folder'

5) Review the module settings using admin/settings/filedepot
   Save your settings and at a minimum, reset to defaults and save settings.
   Review the file type filtering and extension mapping - this sets the allowable files that can be uploaded and the icon that appears in the listing to be associated with the file type.

6) Access the module and run create some initial folders and upload files
   {siteurl}/index.php?q=filedepot    ({siteurl}/filedepot - with clean-urls on)

7) Review the permissions assigned to your site roles: {siteurl}/index.php?q=admin/user/permissions
   Users will need atleast 'access filedepot' permission to access filedepot and to view/download files.

Notes:
a)  Filedepot looks best and works best if you disable all side blocks
b)  You can also create new folders and upload files (attachments) via the native Drupal Content Add/View/Edit interface.
c)  A new content type is automatically created 'filedepot_folder'.


Organic Group Mode and OG settings

  Support for both OG module v1 and v2 branches - refer to the module appropriate setup instructions

  The filedepot module requires both the OG module and the OG Group Access Control modules to be enabled.

  Note: Enable the og_context module if your using the Commons3 distribution to effectively track and pass in the group context to filedepot.

  Enabling the OG option to "Automatically Create Root Level Folder for New Organic Groups" will create a new top level "ROOT" folder
  whenever a new group is created. The group node id (nid) is set only for new groups. If you have existing groups, you can create
  new top level folders and will need to set the group_nid field in the filedepot_categories table to the group node id manually.

  If you want your group members to only see the folders and files under the Group Root level folder, then enable the
  second OG setting "Only Display Organic Group Root Level Folder". If enabled and the users current group context
  is known, only the folders and files under the Group's root folder will be displayed.

  Example URL's to view folders and files in OG Mode (OG Root folder):
    {site_url}/filedepot/{group name}  -     {site_url}/filedepot/sales
      or
    {site_url}/filedepot/{group id}    -     {site_url}/filedepot/23


YUI Javascript Libraries
    The filedepot module uses the YUI javascript (Yahoo User Interface) libraries extensively for the layout manager, dialogs, AJAX, and various other UI and application purposes.
    The javascript libraries are by default loaded from Yahoo at: http://yui.yahooapis.com/combo?2.7.0/build/
    This BASE URL to load the libraries from is defined in the filedepot module online admin configuration settings: admin/settings/filedepot under the Base Setup

    You can setup filedepot to load the YUI libraries from your own site or another server.
    Only version 2.7.0 of the libraries is supported at present.
    You can find YUI 2.7.x version of the complete library on github: https://github.com/yui/yui2/tree/2.7.0
    Link to download complete archive: https://github.com/yui/yui2/archive/2.7.0.zip

    For a SSL (https) supported implementation, you can host the YUI libraries locally or
    review this issue: https://drupal.org/node/2014523 for more details on using a CDN

How to setup to use locally hosted YUI libraries or to load from another server
    Download the complete YUI 2.7.0 library from YUI libraries from http://yuilibrary.com/downloads/#yui2
    Create a folder under sites/all/libraries called yui_2.7.0
    Copy the library folders and files to your sites/all/libraries/yui_2.7.0
    There should be a sites/all/libraries/yui_2.7.0/build directory - with all the individual YUI component subfolders and files from the downloaded archive.
    You can leave all the other YUI directories but only the build directory is required.
    Make sure you have changed the setting under admin/settings/filedepot under Base Setup - the Base URL should be something like: http://www.sitename.com/sites/all/libraries/yui_2.7.0/build/

    Debugging
    Using Firefox, use firebug to see if there is a javascript loading error. If the URL is not correct to load yuiloader.js, you will see a 404 error.
    Possible reasons are: the YUI libraries are not in the correct location, the setting for Base URL is not correct, or the files are not accessible by the webserver.
    If the files are really in the correct directory, check your web server logs and web server security setup.
    Copy a test index.html containing basic html as a test in one of the YUI library directories and enter the URL in your browser to load it.
    Loading the test html file is no different then the browser trying to load the JS library.



