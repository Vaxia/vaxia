/* jQuery driven Eclipse Behaviors. */
(function ($) {
  Drupal.behaviors.themePullouts = {
    attach: function (context, settings) {

  // Add toggle buttons.
  $('.logged-in article.node-rpg-chatroom').once(function() {

    // Setup. Hide all the non-essentials.
    $('.sidebar-first #content').css('margin-left', '0%');
    $('.sidebar-first #content').css('width', '100%');
    $('.sidebar-first .region-sidebar-first').css('width', '100%');
    $('#header').hide();

    // Pullout for Header.
    $('#navigation').wrap('<div id="extruderBarHead" class="{title:\'View Site Nav\'} isClosed jsPullout"></div>');
    $('#extruderBarHead').buildMbExtruder({
      position:"left",
      positionFixed:false,
      top:60,
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
      top:170,
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
    $('article.node-rpg-chatroom .field-name-field-wiki-source').wrap('<div id="extruderBarDesc" class="{title:\'View Description\'} isClosed jsPullout"></div>');
    $('#extruderBarDesc').buildMbExtruder({
      position:"left",
      positionFixed:false,
      top:283,
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
      top:417,
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
  };
})(jQuery);