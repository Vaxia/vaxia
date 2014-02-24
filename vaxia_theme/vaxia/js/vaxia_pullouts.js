/* jQuery driven Eclipse Behaviors. */
(function ($) {
  Drupal.behaviors.themePullouts = {
    attach: function (context, settings) {

  // Only run this if the screen is over 600.
  if ($( window ).width() > 600) {
    // Add toggle buttons.
    $('.logged-in article.node-rpg-chatroom').once(function() {

      // Hide and adjust widths on the sidebar vs content.
      $('.sidebar-first #content').css('margin-left', '0%');
      $('.sidebar-first #content').css('width', '100%');
      $('.sidebar-first .region-sidebar-first').css('width', '100%');
      // Set up sidebar fieldsets that are collapsible and collapsed to be uncollapsed.
      // Because that piece of code doesn't work on this pullout.
      $('.sidebar-first .region-sidebar-first fieldset.collapsed').removeClass('collapsed');
      // And hide the header.
      $('#header').hide();

      // Pullout for Header.
      $('#navigation').wrap('<div id="extruderBarHead" class="{title:\'View Site Nav\'} isClosed jsPullout"></div>');
      $('#extruderBarHead').buildMbExtruder({
        position:"left",
        positionFixed:false,
        top:10,
        extruderOpacity:0.8,
        onExtOpen:function(){
          $('#extruderBarHead').addClass('isOpen');
          $('#extruderBarHead').removeClass('isClosed');
        },
        onExtContentLoad:function(){},
        onExtClose:function(){
          $('#extruderBarHead').removeClass('isOpen');
          $('#extruderBarHead').addClass('isClosed');
        }
      });

      // Pullout for Sidebar.
      $('.sidebar-first .region-sidebar-first').wrap('<div id="extruderBarFirst" class="{title:\'View Sidebar\'} isClosed jsPullout"></div>');
      $('#extruderBarFirst').buildMbExtruder({
        position:"left",
        positionFixed:false,
        top:129,
        extruderOpacity:0.8,
        onExtOpen:function(){
          $('#extruderBarFirst').addClass('isOpen');
          $('#extruderBarHead').removeClass('isClosed');
        },
        onExtContentLoad:function(){},
        onExtClose:function(){
          $('#extruderBarFirst').removeClass('isOpen');
          $('#extruderBarHead').addClass('isClosed');
        }
      });

      // Pullout for Description.
      $('article.node-rpg-chatroom #node_rpg_chatroom_full_group_description').wrap('<div id="extruderBarDesc" class="{title:\'View Description\'} isClosed jsPullout"></div>');
      $('#extruderBarDesc').buildMbExtruder({
        position:"left",
        positionFixed:false,
        top:243,
        extruderOpacity:0.8,
        onExtOpen:function(){
          $('#extruderBarDesc').addClass('isOpen');
          $('#extruderBarDesc').removeClass('isClosed');
        },
        onExtContentLoad:function(){},
        onExtClose:function(){
          $('#extruderBarDesc').removeClass('isOpen');
          $('#extruderBarDesc').addClass('isClosed');
        }
      });

      // Pullout for Room Navigation.
      $('article.node-rpg-chatroom #node_rpg_chatroom_full_group_navigation').wrap('<div id="extruderBarNav" class="{title:\'View Room Nav\'} isClosed jsPullout"></div>');
      $('#extruderBarNav').buildMbExtruder({
        position:"left",
        positionFixed:false,
        top:377,
        extruderOpacity:0.8,
        onExtOpen:function(){
          $('#extruderBarNav').addClass('isOpen');
          $('#extruderBarDesc').removeClass('isClosed');
        },
        onExtContentLoad:function(){},
        onExtClose:function(){
          $('#extruderBarNav').removeClass('isOpen');
          $('#extruderBarHead').addClass('isClosed');
        }
      });

    });
  }
  else {
    // Only run this if the screen is equal or under 480.
    // If it can be collapsed, collapse it.
    $('fieldset.collapsible').addClass('collapsed');
  }

    }
  };
})(jQuery);