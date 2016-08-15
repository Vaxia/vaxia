Overview
--------
This module allows users to hide potential "spoiler" content by inserting them
between [spoiler][/spoiler] tags. These tags will be converted to HTML by this
filter and the relevant CSS rules will set the foreground and background colours
to the same value, thereby rendering the text invisible until highlighted.

If Javascript is available, the spoiler is replaced with a clickable (CSS)
button, which when clicked reveals the content.

The module provides a theme function named theme_spoiler which can be
overridden to make changes to the markup etc. 

Author
---------
Karthik Kumar / Zen / |gatsby| : http://drupal.org/user/21209

Links
-----
Project page: http://drupal.org/project/spoiler
