-------------------------------------------------------------------------------
IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT 
-------------------------------------------------------------------------------

THIS VERSION OF VAXIA CODE IS DEPRECATED. IT IS NO LONGER BEING SUPPORTED.
YOU SHOULD NOT USE IT.
INSTEAD USE IT - AT MOST - AS A MODEL OF CODE AGAINST THE LATEST DRUPAL VERSION.
 
-------------------------------------------------------------------------------
IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT IMPORTANT 
------------------------------------------------------------------------------- 






CONTENTS
--------
Introduction
Installation
Configuration

INTRODUCTION
------------
Drupal was selected as the basis for this as it came with many critical features
already supported: User accounts, login, MySQL database storage, file handling,
display configuration and access control, and ability to be modified.

INSTALLATION
------------
1) Place vaxia folders in YOUR_SITE/sites/all/modules.
1) Place vaxia_theme folders in YOUR_SITE/sites/all/themes.
2) Go to YOUR_SITE/admin/modules and enable the Vaxia module.
3) Approve enabling all pre-requisites. There are a lot of these so depending
on the server resources installation may be incomplete. Check that the Vaxia
module is active on the Modules page before proceeding to step 4.
4) Go to YOUR_SITE/admin/structure/features to refresh the Vaxia feature. Some
installations will encounter an error on install preventing a full load of 
workflow features. Restoring the feature will ensure that the Workflows are 
properly created.
5) Go to YOUR_SITE/admin/appearance to activate the Vaxia theme.
6) Go to YOUR_SITE/admin/structure/block to arrange displays in the theme.
7) Check all other settings under YOUR_SITE/admin/config and YOUR_SITE/admin/
structure

CONFIGURATION
-------------
1) Character sheets should use the Vaxia sysem. Playable states should be set to 
Character evaluation:approved. Only characters in playable states may be used to
post to chatrooms. The Limbo room setting should be configured after Conversion.

2) Workflow - Character sheets, Artwork, and Items should all be set to the
Workflow matching the name of the content type.

3) Character sheet content types should use the Vaxia system.

4) RPG Chatrooms should have Dice enables, and use the Vaxia dice form.

CARE AND FEEDING
----------------
This is Drupal 7 code, and is for the most part completely compatible with Drupal 7
modules. There are two patched modules in the repo - both of which have patches put
into the associated module issue queues:

a) Corresponding Node References - used to keep characters and items matched up with
each other. It needed a cache_refresh patch. Patch at: https://drupal.org/node/1399906

* This patch has been committed, we should schedule the removal of CNR and replacement.

b) Freelinking - used to modify the wiki links for nodetitle plugin to add additional 
ways to lcoate the intended wiki page. Patch at: https://drupal.org/node/2168243
