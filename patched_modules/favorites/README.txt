--------------------------------------------------------------------------------
  Favorites Module for Drupal
  http://drupal.org/project/favorites

  Drupal 6.x Version by:
    Ezra Barnet Gildesgame | ezra-g | http://growingventuresolutions.com
    David Herminghaus | http://drupal.org/user/833794
--------------------------------------------------------------------------------

CONTENTS
========

1. ABOUT
2. INSTALLATION/SETUP
3. USAGE
4. KNOWN ISSUES

1. ABOUT
========
The Favorites allows users with the appropriate permission to add any page on
the site to "user favorites" block. Favorites are stored as a path (such as
'search') with the query string (such as '?abc=123&def=234'). 

2. INSTALLATION/SETUP
=====================
Just install the module as usual, set the user permissions and place the block
into the region of your choice. Done.

3. USAGE
========
* Adding a page:
  Open the "add this page" section, eventually modify the proposed page title
  and click "add".
* Removing a page:
  Click on the trailing "x".
* Altering an item's title:
  Navigate to the target and simply add it again.

4. KNOW ISSUES
==============
If you are using any extensions which modify form submit buttons or otherwise
dynamically act on block content (e.g. http://drupal.org/project/blajax ),
make sure they will not interfere with the "add favorites" form.

E.g. for the "hide submit" module, make sure you add

favorites_add_favorite_form

to the "Form-id exclusion list" which can be found at admin/settings/hide-submit
within the "advanced" section near the page bottom. Otherwise, strange results
might occur.

Alternatively to all this, you may disable Ajax support in the favorites block settings
(see admin/build/block/configure/favorites/0).
