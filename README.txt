CONTENTS
--------
Introduction
Installation
Configuration
Conversion of old Vaxia

INTRODUCTION
------------
Vaxia has operated for ages on old software with critical security flaws
and non-standard file structures. It has become difficult to maintain, and 
extremely difficult to modify. Thankfully, we have other options.

Drupal was selected as the basis for this as it came with many critical features
already supported: User accounts, login, MySQL database storage, file handling,
display configuration and access control, and ability to be modified.

Adding Vaxia specific features was simply a matter of adding modules to handle
Vaxia specific features. Using the Character Sheet, Dice Roller, and the Zen
base theme Vaxia can be replicated in Drupal while gaining all the benefits.

The three major components of this effort are included in this repo.

Vaxia - the game engine, handles character sheets, dice rolls. This handles the 
math of character creation and coordinates the game mechanics on rolls.

RPG Chat - Contained within the Vaxia module, adds JS behaviors to Drupal 
comments to create a chat-room interface. (Optional)

Vaxia Conversion - Creates XML files out of the old Vaxia file system, and 
handles the import of that information into Drupal, including image files.

INSTALLATION
------------
1) Place vaxia and vaxia_convert folders in YOUR_SITE/sites/all/modules.
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

