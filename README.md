Match Redirect
--------------

This module provides redirecting based on path patterns with wildcards. This 
functions much like how block page visibility works. You specify a pattern like 
"old-blog/*" and a target like "new-blog" and all pages under old-blog will be 
redirected to new-blog.

Features
--------

* Pattern matching allowing wildcard redirects
* Redirect code choice (301, 302, etc)
* Filters out existing pages so they won't be redirected (unless overridden)
* Loop protection (no chaining redirects allowed)


Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.

- Visit the configuration page under Administration > Configuration > URL Handling >
  Match Redirects (admin/config/urls/match_redirect) and enter the required information.


Issues
------

Bugs and Feature requests should be reported in the Issue Queue:
https://github.com/backdrop-contrib/match_redirect/issues.

Current Maintainers
-------------------

- Kevin Reynen (https://github.com/kreynen).


Credits
-------

- Ported to Backdrop CMS by Kevin Reynen (https://github.com/kreynen).
- Originally written for Drupal by biff45452 (https://www.drupal.org/u/biff45452).

License
-------

This project is GPL-2.0 software. See the LICENSE.txt file in this directory for
complete text.

