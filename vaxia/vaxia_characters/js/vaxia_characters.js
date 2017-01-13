/* jQuery driven theme Behaviors. */
Drupal.behaviors.vaxia_characters = {
  attach: function(context, settings) { (function($) {

  /* Add buttons for the javascript preloads. */
  $('document').ready(function() {

    function vaxia_characters_init() {
        var template_form = '' +
        '<div class="vaxia-character-sheet-template-feedback messages--status messages status"></div>' +
        '<fieldset class="collapsible form-wrapper vaxia-character-sheet-templates" id="edit-field-templates">' +
        '<legend><span class="fieldset-legend">' +
        '<a class="fieldset-title" href="#"><span class="fieldset-legend-prefix element-invisible">Hide</span> Character wizard</a>' +
        '<span class="summary"></span></span></legend>' +
        '<div class="fieldset-wrapper"><div class="fieldset-description">' +
        '<h3>Would you like to make this quick as possible? Awesome - start here!</h3>' +
        '<div class="vaxia-character-sheet-template-desc">Click on a button to preload the stats and skills for a type of character. ' +
        'We will pre-fill your form for a head start so you can focus on creating the background and personality.</div>' +
        '<div class="vaxia-character-sheet-setting">' +
        '<div class="vaxia-character-sheet-template-label">Fantasy Concepts</div>' +
        '<ul class="vaxia-character-sheet-template-buttons">' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="vaxia" template="mage" href="#">Mage</a></li>' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="vaxia" template="rogue" href="#">Rogue</a></li>' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="vaxia" template="fighter" href="#">Fighter</a></li>' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="vaxia" template="healer" href="#">Healer</a></li>' +
        '</ul></div>' +
        '<div class="vaxia-character-sheet-setting">' +
        '<div class="vaxia-character-sheet-template-label">Science Fiction Concepts</div>' +
        '<ul class="vaxia-character-sheet-template-buttons">' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="sirian" template="engineer" href="#">Engineer</a></li>' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="sirian" template="hacker" href="#">Hacker</a></li>' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="sirian" template="gunbunny" href="#">Gunbunny</a></li>' +
        '<li><a class="button vaxia-character-sheet-template-button" realm="sirian" template="medic" href="#">Medic</a></li>' +
        '</ul></div>' +
        '</div></div>' +
        '</fieldset>';
        $('.node-character_sheet-form').prepend(template_form);
        $('.vaxia-character-sheet-template-feedback').hide();
    }

    function vaxia_characters_template_fill(clicked) {
      var template = $(clicked).attr('template');
      var realm = $(clicked).attr('realm');
      var realmName = realm.charAt(0).toUpperCase() + realm.slice(1);
      $('.vaxia-character-sheet-template-feedback').html('You selected a ' + template + ' which is a ' + realmName + ' concept. ' +
        'Please allow a moment for us to fill out the sheet for you. ' +
        'Since this is randomly generated you may need to review it to make sure it makes sense.').show();
      // General details.
      $('#character-sheet-node-form #edit-field-tag-realm-und').val(vaxia_characters_template_realm(realm, template)).change();
      var gender = vaxia_characters_template_gender(realm, template);
      var species = vaxia_characters_template_species(realm, template);
      $('#character-sheet-node-form #edit-title').val(vaxia_characters_template_name(realm, template));
      $('#character-sheet-node-form #edit-field-last-name-und-0-value').val(vaxia_characters_template_name(realm, template));
      $('#character-sheet-node-form #edit-field-background-und-0-value').val(vaxia_characters_template_background(realm, template, gender, species));
      $('#character-sheet-node-form #edit-field-personality-und-0-value').val(vaxia_characters_template_personality(realm, template, gender, species));
      $('#character-sheet-node-form #edit-field-occupation-und-0-value').val(vaxia_characters_template_occupation(realm, template, gender, species));
      $('#character-sheet-node-form #edit-field-description-und-0-value').val(vaxia_characters_template_description(realm, template, gender, species));
      $('#character-sheet-node-form #edit-field-age-app-und-0-value').val(18);
      // Set the stats and skills.
      vaxia_characters_template_stats(realm, template);
      vaxia_characters_template_skills(realm, template);
      vaxia_characters_template_langs(realm, template);

      // Close the wizard.
      $('.vaxia-character-sheet-templates .fieldset-legend .fieldset-title').click();
    }

    function vaxia_characters_template_realm(realm, template) {
      // Default by realm.
      if (realm == 'sirian') {
        return 726;
      }
      return 725;
    }

    function vaxia_characters_template_species(realm, template) {
      // Default by realm.
      if (realm == 'sirian') {
        var species = ['human', 'human', 'human', 'human', 'human', 'human','metalborn'];
        species = vaxia_characters_template_random(species);
        $('#character-sheet-node-form #edit-field-sirian-species-und').val(species);
      }
      else {
        var species = ['human', 'human', 'human', 'half-elf', 'half-elf', 'half-elf',
          'elf', 'elf',  'dwarf', 'orc', 'half-orc', 'gnome'];
        species = vaxia_characters_template_random(species);
        $('#character-sheet-node-form #edit-field-race-und').val(species);
      }
      return species;
    }

    function vaxia_characters_template_gender(realm, template) {
      var genders = ['male', 'female', 'hard to tell'];
      var gender = vaxia_characters_template_random(genders);
      // Set the value for the form.
      $('#character-sheet-node-form #edit-field-sex-und-0-value').val(gender);
      // Get the value for the final search and replace.
      // Dev note: Apologies here, I know this isn't accurate *at all* for NB but for the
      // sake of simplicity and purposes of getting players started fast, needed. You can still
      // change the pronouns by editing the generated text and apparent gender display.
      if (gender == 'hard to tell') {
        // Hard to tell option.
        var random = Math.round(Math.random());
        gender = 'male';
        if (random) {
          gender = 'female';
        }
      }
      return gender;
    }

    function vaxia_characters_template_name(realm, template) {
      var first = [
'a', 'e', 'i', 'o', 'u', 'y', 'ace', 'ach', 'ada', 'adh', 'aed', 'ael', 'aer', 'agh', 'aid',
'aig', 'ail', 'ain', 'air', 'ais', 'ait', 'ala', 'ald', 'all', 'ana', 'and', 'ane', 'ang',
'ani', 'ann', 'ans', 'ant', 'any', 'ara', 'ard', 'are', 'ari', 'arn', 'arr', 'art', 'ase',
'ast', 'ath', 'aug', 'aur', 'ayn', 'ban', 'bar', 'bba', 'bel', 'ber', 'bes', 'bha', 'bin',
'ble', 'bor', 'bus', 'can', 'cca', 'cey', 'cha', 'che', 'cht', 'chu', 'cia', 'cie', 'cus',
'dal', 'dan', 'dar', 'dda', 'ddi', 'del', 'den', 'der', 'des', 'dez', 'dge', 'dha', 'dhe',
'die', 'dig', 'dil', 'din', 'dir', 'dis', 'doc', 'don', 'dor', 'dra', 'dre', 'dri', 'dui',
'dur', 'dus', 'dyn', 'eah', 'ean', 'ech', 'eda', 'edd', 'ede', 'een', 'ein', 'eir', 'eis',
'ela', 'eld', 'ele', 'ell', 'elm', 'ena', 'ene', 'enn', 'ent', 'era', 'erd', 'ere', 'erg',
'eri', 'ern', 'ers', 'ert', 'ery', 'esa', 'ess', 'eta', 'ete', 'eth', 'ett', 'eus', 'eva',
'eve', 'eyn', 'fer', 'fin', 'fur', 'fus', 'gal', 'gan', 'gar', 'gel', 'gen', 'ger', 'ges',
'ght', 'gil', 'gin', 'gis', 'gol', 'gon', 'gor', 'gur', 'gus', 'had', 'hal', 'ham', 'han',
'har', 'has', 'hbh', 'hed', 'hel', 'hen', 'her', 'hes', 'het', 'hia', 'hil', 'hin', 'hir',
'his', 'hna', 'hne', 'hon', 'hor', 'hos', 'hri', 'hrt', 'hta', 'hus', 'ial', 'iam', 'ian',
'ias', 'ibe', 'ica', 'ice', 'ich', 'ick', 'ico', 'ida', 'ide', 'idh', 'iel', 'ien', 'ier',
'ies', 'iet', 'ige', 'igh', 'ila', 'ild', 'ile', 'ili', 'ill', 'ilo', 'ime', 'ina', 'ind',
'ine', 'ing', 'ink', 'inn', 'ins', 'ion', 'ior', 'ios', 'ira', 'ire', 'isa', 'ise', 'ish',
'iss', 'ist', 'ita', 'ite', 'ith', 'itt', 'iua', 'ius', 'iva', 'kin', 'kur', 'lad', 'laf',
'lah', 'lan', 'lar', 'las', 'lda', 'lde', 'leb', 'led', 'lee', 'leg', 'len', 'ler', 'les',
'ley', 'lia', 'lie', 'lig', 'lim', 'lin', 'lio', 'lis', 'lla', 'lle', 'lli', 'llt', 'lly',
'lma', 'lon', 'lor', 'los', 'lph', 'lta', 'lte', 'lum', 'lus', 'lyn', 'lys', 'man', 'mar',
'mas', 'men', 'mer', 'mes', 'mir', 'mma', 'mon', 'mus', 'nah', 'nai', 'nal', 'nan', 'nar',
'nas', 'nat', 'nce', 'nda', 'nde', 'ndi', 'ndy', 'nea', 'ned', 'nee', 'nel', 'nen', 'ner',
'nes', 'net', 'ney', 'nez', 'nge', 'nia', 'nie', 'nig', 'nil', 'nin', 'nir', 'nis', 'nna',
'nne', 'nni', 'nny', 'noc', 'nod', 'non', 'nor', 'nos', 'nsa', 'nse', 'nta', 'nte', 'nth',
'nui', 'nur', 'nus', 'nya', 'nye', 'och', 'ock', 'oda', 'oel', 'oin', 'oke', 'ola', 'old',
'olf', 'oll', 'ome', 'ona', 'ond', 'one', 'oni', 'onn', 'ood', 'ora', 'ord', 'ore', 'ori',
'orn', 'ort', 'ose', 'ost', 'ota', 'ote', 'oth', 'our', 'ppa', 'pre', 'que', 'rad', 'rah',
'rai', 'ram', 'ran', 'ras', 'rch', 'rda', 'rde', 'rea', 'red', 'ree', 'rel', 'ren', 'res',
'ret', 'rey', 'rga', 'rht', 'ria', 'ric', 'rid', 'rie', 'rig', 'rik', 'ril', 'rim', 'rin',
'rio', 'ris', 'rit', 'rix', 'rla', 'rle', 'rly', 'rna', 'rne', 'rni', 'rod', 'rog', 'ron',
'ros', 'rra', 'rre', 'rri', 'rry', 'rse', 'rta', 'rth', 'rts', 'rum', 'run', 'rus', 'ryl',
'ryn', 'rys', 'sar', 'sel', 'sen', 'ser', 'ses', 'set', 'sey', 'sha', 'sia', 'sie', 'sin',
'sis', 'son', 'ssa', 'sse', 'sta', 'ste', 'sth', 'sus', 'tan', 'tar', 'tel', 'ten', 'ter',
'tes', 'tet', 'tha', 'the', 'thi', 'tia', 'tie', 'tin', 'tio', 'tis', 'ton', 'tor', 'tos',
'tta', 'tte', 'tun', 'tur', 'tus', 'uan', 'uda', 'uel', 'uen', 'ugh', 'uil', 'uin', 'ula',
'ulf', 'una', 'und', 'unt', 'ure', 'urg', 'urn', 'ury', 'ust', 'uth', 'val', 'van', 'var',
'vel', 'ven', 'ver', 'vie', 'vin', 'vis', 'von', 'vor', 'vyn', 'wan', 'wel', 'wen', 'win',
'wyn', 'yan', 'yce', 'ydd', 'yna', 'yne', 'ynn', 'yon', 'yth', 'zel'
      ];
      var name = vaxia_characters_template_string(first, first, 2, 1, '');
      return name.charAt(0).toUpperCase() + name.slice(1);
    }

    function vaxia_characters_template_background(realm, template, gender, species) {
      var first = [
' disturbing ',
' dreadful ',
' gruesome ',
' horrifying ',
' shocking ',
' terrible ',
' tormented ',
' troubled ',
' ugly ',
' unsettling ',
' typical ',
' wealthy ',
' royal',
' fairly rich ',
' high class ',
' middle class ',
' loving ',
' large ',
' small ',
' decent ',
' successful'
      ];
      var second = [
' town ',
' city ',
' village ',
' port ',
' community ',
' capital ',
' commune ',
' orphanage ',
' street '
      ];
      var background = ' Ze grew up in a ' + vaxia_characters_template_string(first, second, 1, 1, '');
      first = [
' without worry until ze was about ',
' out of trouble until ze was about ',
' free of worries until ze was about ',
' free of trouble until ze was about ',
' in peace until ze was about ',
' comfortably until ze was about ',
' happily until ze was about ',
' a rough life until ze was about ',
' a quiet life until ze was about ',
' in poverty until ze was about '
      ];
      second = [
' 7 ',
' 10 ',
' 12 ',
' 15 ',
' 18 '
      ];
      background = background + ' and lived' + vaxia_characters_template_string(first, second, 1, 1, '') + 'years old. ';
      first = [
' gained responsibilities after ',
' gained new responsibilities after ',
' started to travel a lot after ',
' started to travel the world after ',
' started to experience the world ',
' started working for the family company after ',
' studied a lot after ',
' went to school after ',
' explored the country after ',
' moved out after ',
' moved in with a friend after ',
' did volunteering work after ',
' did a lot of small jobs after ',
' moved to another country after ',
' became a travelling adventurer after ',
' got a new pet after ',
' got a new companion after ',
' lost zir parents in ',
' lost zir mother in ',
' lost zir father in ',
' lost zir parents when they left after ',
' lost zir best friend in ',
' lost zir home when it was destroyed after ',
' lost zir money after ',
' lost zir family was they were split up after ',
' lost zir brother in ',
' lost zir sister in ',
' lost zir sisters in ',
' lost zir brothers in ',
' lost zir siblings in ',
' lost zir family in ',
' killed somebody by accident during ',
' killed somebody during ',
' maimed somebody during ',
' accidentally maimed somebody during ',
' destroyed someone\'s life during ',
' destroyed someone\'s life by accident during '
];
      second = [
' a freak fire.',
' a robbery gone wrong.',
' a terrible disaster.',
' a natural disaster.',
' a suspicious accident.',
' a fight which got out of control.',
' an invasion.',
' a brutal war.',
' a drought.',
' an act of terrorism.',
' a volcanic eruption.',
' a hurricane.',
' an earthquake.',
' a horrible flood.',
' a long lasting heatwave.',
' an epidemic.',
' a food shortage.',
' a power outage.',
' a government takeover.',
' a rebellion.',
' a revolution.'
      ];
      background = background + 'Then ze ' + vaxia_characters_template_string(first, second, 1, 1, '');
      first = [
' with the support of great friends ',
' alongside great friends ',
' with amazing, new friends ',
' with a great deal of determination ',
' with determination and some luck ',
' alongside trusted friends ',
' with a great companion ',
' through hard work ',
' through plenty of trial and error ',
' by never giving up ',
' after an astonishing adventure ',
' with the help of great friends ',
' having overcome plenty of obstacles ',
' with a new found friend ',
' all alone ',
' without any help ',
' with a childhood friend ',
' with the help of a stranger ',
' alone, lost and forgotten ',
' with the help of a small group of strangers ',
' with a couple of friends ',
' with a new found love ',
' with the help of a suspicious stranger ',
' with the help of a suspicious friend ',
' while pursued by the authority ',
' while pursued by a criminal gang ',
' while pursued by strangers ',
' while obstructed by many ',
' against all odds ',
' alongside a brother ',
' alongside a sister ',
' alongside a cousin ',
' together with a companion ',
' together with a pet ',
' with a loyal friend ',
' reunited with a friend ',
' reunited with a lost pet '
      ];
      second = [
' ze is reaching great success.',
' ze is finding a way to the top.',
' ze is fulfilling all dreams.',
' ze is accomplish all goals.',
' ze is improving the world.',
' ze is going beyond expectations.',
' ze is climbing to the top.',
' ze is staying ahead of the game.',
' ze is reaching full potential.',
' ze is overcoming all odds.',
' ze is surviving everything.',
' ze is going beyond expectations.',
' ze is facing all obstacles.',
' ze is conquering all fears and doubts.',
' ze is crushing all that\'s in the way.',
' ze is overpowering anybody who\'s a hinderance.',
' ze is keeping ahead of the curve.',
' ze is remaining out of reach of danger.',
' ze is training to perfection.',
' ze is reaching full potential.',
' ze is starting a new life.',
' ze is finding a new home.',
' ze is a force to be reckoned with.',
' ze is a true friend for life.',
' ze is an ally you\'d want by your side.',
' ze is somebody we can expect great things of.',
' ze is somebody who could change the world.',
' ze is an unstoppable force.',
' ze is a friend you\'d want by your side.'
      ];
      background = background + ' Now ' + vaxia_characters_template_string(first, second, 1, 1, '');
      return vaxia_characters_template_personalize(background, gender, species);
    }

    function vaxia_characters_template_personality(realm, template, gender, species) {
      var first = [
' Ze is an adventurous young SPECIES ',
' Ze is an affectionate young SPECIES ',
' Ze is an analytical young SPECIES ',
' Ze is an athletic young SPECIES ',
' Ze is a brave young SPECIES ',
' Ze is a calm young SPECIES ',
' Ze is a capable young SPECIES ',
' Ze is a charismatic young SPECIES ',
' Ze is a charming young SPECIES ',
' Ze is a cheerful young SPECIES ',
' Ze is a creative young SPECIES ',
' Ze is a curious young SPECIES ',
' Ze is a daring young SPECIES ',
' Ze is a dedicated young SPECIES ',
' Ze is a dependable young SPECIES ',
' Ze is a determined young SPECIES ',
' Ze is a driven young SPECIES ',
' Ze is a dutiful young SPECIES ',
' Ze is an eager young SPECIES ',
' Ze is an elegant young SPECIES ',
' Ze is an energetic young SPECIES ',
' Ze is a faithful young SPECIES ',
' Ze is a funny young SPECIES ',
' Ze is a generous young SPECIES ',
' Ze is a gentle young SPECIES ',
' Ze is a happy young SPECIES ',
' Ze is a helpful young SPECIES ',
' Ze is a honest young SPECIES ',
' Ze is a hospitable young SPECIES ',
' Ze is a humble young SPECIES ',
' Ze is a humorous young SPECIES ',
' Ze is an innocent young SPECIES ',
' Ze is an intelligent young SPECIES ',
' Ze is an intrepid young SPECIES ',
' Ze is a jovial young SPECIES ',
' Ze is a just young SPECIES ',
' Ze is a light-hearted young SPECIES ',
' Ze is a loyal young SPECIES ',
' Ze is a modest young SPECIES ',
' Ze is a mysterious young SPECIES ',
' Ze is a polite young SPECIES ',
' Ze is a popular young SPECIES ',
' Ze is a proud young SPECIES ',
' Ze is a quick young SPECIES ',
' Ze is a reliable young SPECIES ',
' Ze is a responsible young SPECIES ',
' Ze is a savvy young SPECIES ',
' Ze is a sensitive young SPECIES ',
' Ze is a sincere young SPECIES ',
' Ze is a sweet young SPECIES ',
' Ze is a talkative young SPECIES ',
' Ze is a thoughtful young SPECIES ',
' Ze is a whimsical young SPECIES ',
' Ze is a wise young SPECIES ',
' Ze is a witty young SPECIES '
      ];
      var second = [
' with anxious tendancies. ',
' with arrogant tendancies. ',
' with bewildered tendancies. ',
' with bossy tendancies. ',
' with conceited tendancies. ',
' with confused tendancies. ',
' with facetious tendancies. ',
' with foolish tendancies. ',
' with greedy tendancies. ',
' with grouchy tendancies. ',
' with harsh tendancies. ',
' with ignorant tendancies. ',
' with immature tendancies. ',
' with impatient tendancies. ',
' with impulsive tendancies. ',
' with jealous tendancies. ',
' with lonely tendancies. ',
' with mean tendancies. ',
' with naive tendancies. ',
' with nervous tendancies. ',
' with opinionated tendancies. ',
' with pompous tendancies. ',
' with rash tendancies. ',
' with restless tendancies. ',
' with rude tendancies. ',
' with selfish tendancies. ',
' with snobbish tendancies. ',
' with stubborn tendancies. ',
' with timid tendancies. ',
' with uncontrolled tendancies. '
      ];
      var personality = vaxia_characters_template_string(first, second, 1, 1, '');
      first = [
' This is to be expected from someone ',
' But what else do you expect from someone ',
' This is not surprising considering for someone ',
' Which is not out of the ordinary for someone ',
' But this is all just a facade, for someone ',
' But there is more than this to someone ',
' But there is more than meets the eye, not surprising for someone '
      ];
      second = [
' with zir past. ',
' with zir potential. ',
' with zir future plans. ',
' with zir raising. ',
      ];
      personality = personality + vaxia_characters_template_string(first, second, 1, 1, '');
      return vaxia_characters_template_personalize(personality, gender, species);
    }

    function vaxia_characters_template_occupation(realm, template, gender, species) {
      var first = [
' strange ',
' weird ',
' ever changing ',
' fast ',
' fast changing ',
' amazing ',
' fancy ',
' fantastic ',
' wacky ',
' absurd ',
' strange ',
' wild ',
' remarkable ',
' wonderful ',
' outlandish ',
' astonishing ',
' extraordinary ',
' mystifying ',
' wicked ',
' bizarre ',
' cruel ',
' outlandish ',
' odd ',
' harsh ',
' criminal ',
' bitter ',
' rough ',
' bleak ',
' brutal ',
' relentless ',
' unkind ',
' pitiless ',
' vicious ',
' villainous ',
' corrupt '
      ];
      var occupation = ' Ze is a ' + vaxia_characters_template_string(first, first, 1, 1, ' and ') + template + '.';
      return vaxia_characters_template_personalize(occupation, gender, species);
    }

    function vaxia_characters_template_description(realm, template, gender, species) {
      var first = [
'very tall ',
'tall ',
'average height ',
'short ',
'very short '
      ];
      var second = [
' with blue ',
' with green ',
' with brown ',
' with brown ',
' with brown ',
' with brown ',
' with light blue ',
' with light blue ',
' with hazel ',
' with light green ',
' with chestnut ',
' with chestnut ',
' with chestnut ',
' with silver '
      ];
      var description = ' Ze is ' + vaxia_characters_template_string(first, second, 1, 1, '') + 'eyes.';
      first = [
' short ',
' short spiky ',
' short bristly ',
' well groomed ',
' well groomed ',
' crinkly ',
' sleek ',
' flowing ',
' shaggy ',
' long ',
' curly ',
' straight ',
' wavy ',
' wavy ',
' wavy ',
' frizzy ',
' frizzy ',
' frizzy ',
' long ',
' curly ',
' curly ',
' coily ',
' shoulder-length '
      ];
      second = [
' purple ',
' blue ',
' green ',
' red ',
' red ',
' red ',
' white ',
' brown ',
' light blue ',
' light green ',
' pink ',
' golden ',
' black ',
' black ',
' black ',
' black ',
' black ',
' gray ',
' white ',
' blonde ',
' brown ',
' brown ',
' brown ',
' brown ',
' red ',
' ginger ',
' chestnut ',
' chestnut ',
' chestnut ',
' chestnut ',
' chestnut ',
' silver ',
' striped '
      ];
      description = description + ' Ze has ' + vaxia_characters_template_string(first, second, 1, 1, '') + ' hair.';
      return vaxia_characters_template_personalize(description, gender, species);
    }

    function vaxia_characters_template_stats(realm, template) {
      // And set the values for each template.
      switch(template) {
        case 'mage':
          var stats = [25,25,20,30,40,35,25];
        break;
        case 'rogue':
          var stats = [25,20,40,35,30,25,25];
        break;
        case 'fighter':
          var stats = [25,30,40,35,25,25,20];
        break;
        case 'healer':
          var stats = [25,25,20,25,40,35,30];
        break;
        case 'engineer':
          var stats = [25,25,25,35,40,30,20];
        break;
        case 'hacker':
          var stats = [25,25,20,35,40,25,30];
        break;
        case 'gunbunny':
          var stats = [25,30,35,40,25,25,20];
        break;
        case 'medic':
          var stats = [25,25,20,35,40,30,25];
        break;
      }
      // Set the given stats.
      $('#character-sheet-node-form #edit-field-life-und-0-value').val(stats[0]);
      $('#character-sheet-node-form #edit-field-endurance-und-0-value').val(stats[1]);
      $('#character-sheet-node-form #edit-field-strength-und-0-value').val(stats[2]);
      $('#character-sheet-node-form #edit-field-dexterity-und-0-value').val(stats[3]);
      $('#character-sheet-node-form #edit-field-intelligence-und-0-value').val(stats[4]);
      $('#character-sheet-node-form #edit-field-spirituality-und-0-value').val(stats[5]);
      $('#character-sheet-node-form #edit-field-charisma-und-0-value').val(stats[6]);
    }

    function vaxia_characters_template_skills(realm, template) {
      // Set up two skills. @todo - switch to set interval.
      $('#character-sheet-node-form #edit-field-skill-und-add-more').trigger('mousedown');
      var skill2 = setTimeout(function() {
        $('#character-sheet-node-form #edit-field-skill-und-add-more--2').trigger('mousedown');
      }, 3000);
      var skill3 = setTimeout(function() {
        _vaxia_characters_template_skills(realm, template);
      }, 6000);
    }

    function _vaxia_characters_template_skills(realm, template) {
      // Update the feedback.
      $('.vaxia-character-sheet-template-feedback').html('Your sheet is ready. Enjoy!').delay('2500').fadeOut(2500);
      // And set the values for each template.
      switch(template) {
        case 'mage':
          var skillname = ['Elementalist', 'Spellcraft', 'Item Craft'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to use the element of fire to attack, defend, and shield others.',
'The ability to understand principles of magic, and analyze magical effects in the field.',
'The ability to create magical items to lock in the spells of others or your own.'
          ];
          var aspect1 = ['1145', '1131', '1127'];
          var aspect2 = ['1141', '1130', '1129'];
          var aspect3 = ['1142', '1130', '1128'];
        break;
        case 'rogue':
          var skillname = ['Acrobatics', 'Fighting', 'Alertness'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to move quickly, quietly, and acrobatically.',
'The ability to use a dagger to attack and defend, and to maintain the weapon.',
'The ability to use eyes and ears to detect trouble, and avoid traps.'
          ];
          var aspect1 = ['1143', '1145', '1131'];
          var aspect2 = ['1141', '1141', '1131'];
          var aspect3 = ['1134', '1129', '1144'];
        break;
        case 'fighter':
          var skillname = ['Guns', 'Martial Arts', 'Taunt'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to use a sword to attack and defend, and to maintain the weapon.',
'The ability to fight barehanded and anticipate opponents in a fight.',
'The ability to read people, discourage them, and convince them of lies.'
          ];
          var aspect1 = ['1145', '1145', '1131'];
          var aspect2 = ['1141', '1141', '1138'];
          var aspect3 = ['1129', '1139', '1140'];
        break;
        case 'healer':
          var skillname = ['Healing', 'Charismatic', 'Fighting'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to heal others, shield them from harm, and detect illnesses.',
'The ability to read people, encourage them, and convince them to follow you.',
'The ability to use a staff to attack and defend, and to maintain the weapon.'
          ];
          var aspect1 = ['1129', '1131', '1145'];
          var aspect2 = ['1142', '1138', '1141'];
          var aspect3 = ['1131', '1139', '1129'];
        break;
        case 'engineer':
          var skillname = ['Repair', 'Programming', 'Drones'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to repair engines, craft new ones, and understand ones encountered in the field.',
'The ability to repair programs, craft new ones, and understand ones encountered in the field.',
'The ability to operate and repair a drone as a companion. (Comes with free companion!)'
          ];
          var aspect1 = ['1129', '1129', '1133'];
          var aspect2 = ['1127', '1127', '1129'];
          var aspect3 = ['1130', '1130', '1127'];
        break;
        case 'hacker':
          var skillname = ['Hacking', 'Alertness', 'Guns'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to bypass protections on systems, gather information and weaken technical defenses.',
'The ability to use eyes and ears to detect trouble, and avoid traps.',
'The ability to use a gun to attack and defend, and to maintain the weapon.'
          ];
          var aspect1 = ['1131', '1131', '1145'];
          var aspect2 = ['1144', '1131', '1141'];
          var aspect3 = ['1140', '1144', '1129'];
        break;
        case 'gunbunny':
          var skillname = ['Guns', 'Martial Arts', 'Taunt'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to use a gun to attack and defend, and to maintain the weapon.',
'The ability to fight barehanded and anticipate opponents in a fight.',
'The ability to read people, discourage them, and convince them of lies.'
          ];
          var aspect1 = ['1145', '1145', '1131'];
          var aspect2 = ['1141', '1141', '1138'];
          var aspect3 = ['1129', '1139', '1140'];
        break;
        case 'medic':
          var skillname = ['Healing', 'Charismatic', 'Guns'];
          var skillval = [40, 30, 30];
          var skilldesc = [
'The ability to heal others, shield them from harm, and detect illnesses.',
'The ability to read people, encourage them, and convince them to follow you.',
'The ability to use a gun to attack and defend, and to maintain the weapon.'
          ];
          var aspect1 = ['1129', '1131', '1145'];
          var aspect2 = ['1142', '1138', '1141'];
          var aspect3 = ['1131', '1139', '1129'];
        break;
      }
      // Set the given skills. Name.
      $('#character-sheet-node-form #edit-field-skill-und-0-field-skill-name-und-0-value--3').val(skillname[0]);
      $('#character-sheet-node-form #edit-field-skill-und-1-field-skill-name-und-0-value--2').val(skillname[1]);
      $('#character-sheet-node-form #edit-field-skill-und-2-field-skill-name-und-0-value').val(skillname[2]);
      // Value.
      $('#character-sheet-node-form #edit-field-skill-und-0-field-skill-value-und-0-value--3').val(skillval[0]);
      $('#character-sheet-node-form #edit-field-skill-und-1-field-skill-value-und-0-value--2').val(skillval[1]);
      $('#character-sheet-node-form #edit-field-skill-und-2-field-skill-value-und-0-value').val(skillval[2]);
      // Description.
      $('#character-sheet-node-form #edit-field-skill-und-0-field-skill-desc-und-0-value--3').val(skilldesc[0]);
      $('#character-sheet-node-form #edit-field-skill-und-1-field-skill-desc-und-0-value--2').val(skilldesc[1]);
      $('#character-sheet-node-form #edit-field-skill-und-2-field-skill-desc-und-0-value').val(skilldesc[2]);
      // Aspect1.
      $('#character-sheet-node-form #edit-field-skill-und-0-field-aspect-one-und--3').val(aspect1[0]);
      $('#character-sheet-node-form #edit-field-skill-und-1-field-aspect-one-und--2').val(aspect1[1]);
      $('#character-sheet-node-form #edit-field-skill-und-2-field-aspect-one-und').val(aspect1[2]);
      // Aspect2.
      $('#character-sheet-node-form #edit-field-skill-und-0-field-aspect-two-und--3').val(aspect2[0]);
      $('#character-sheet-node-form #edit-field-skill-und-1-field-aspect-two-und--2').val(aspect2[1]);
      $('#character-sheet-node-form #edit-field-skill-und-2-field-aspect-two-und').val(aspect2[2]);
      // Aspect3.
      $('#character-sheet-node-form #edit-field-skill-und-0-field-aspect-three-und--3').val(aspect3[0]);
      $('#character-sheet-node-form #edit-field-skill-und-1-field-aspect-three-und--2').val(aspect3[1]);
      $('#character-sheet-node-form #edit-field-skill-und-2-field-aspect-three-und').val(aspect3[2]);
    }

    function vaxia_characters_template_langs(realm, template) {
      // Default by realm.
      if (realm == 'sirian') {
        $('#character-sheet-node-form #edit-field-sirian-languages-und-dal-knw').attr('checked', true);
        $('#character-sheet-node-form #edit-field-sirian-languages-und-roh-knw').attr('checked', true);
        $('#character-sheet-node-form #edit-field-sirian-languages-und-cln-knw').attr('checked', true);
      }
      else {
        $('#character-sheet-node-form #edit-field-vaxia-languages-und-vax-knw').attr('checked', true);
        $('#character-sheet-node-form #edit-field-vaxia-languages-und-ink-knw').attr('checked', true);
        $('#character-sheet-node-form #edit-field-vaxia-languages-und-elv-nat').attr('checked', true);
      }
    }

    function vaxia_characters_template_string(first, second, max, min, join) {
      var string = '';
      for (i = 1; i <= max; i++) {
        var rand = Math.floor( Math.random() * first.length );
        string = string + first[rand];
        first.splice(rand, 1);
        var rand = Math.floor( Math.random() * second.length );
        string = string + join + second[rand];
        second.splice(rand, 1);
        // Option to return early.
        if (i >= min && i <= max) {
          var early = Math.round(Math.random());
          if (early == 1) {
            return string;
          }
        }
      }
      // Done, return.
      return string;
    }

    function vaxia_characters_template_random(items) {
      return items[Math.floor( Math.random() * items.length )];
    }

    function vaxia_characters_template_personalize(string, gender, species) {
      // Figure out pronouns for replacement.
      var pronouns = [' He ', ' His ', ' he ', ' his '];
      if (gender == 'female') {
        var pronouns = [' She ', ' Her ', ' she ', ' her '];
      }
      // Search and replace the string.
      string = ' ' + string + ' ';
      string = '' + string.replace(/ Ze /g, pronouns[0]);
      string = '' + string.replace(/ Zir /g, pronouns[1]);
      string = '' + string.replace(/ ze /g, pronouns[2]);
      string = '' + string.replace(/ zir /g, pronouns[3]);
      string = '' + string.replace(/SPECIES/g, species);
      string = '' + string.replace(/  /g, ' ');
      string = '' + string.replace(/  /g, ' ');
      string = '' + string.replace(/  /g, ' ');
      return string.trim();
    }

    $('.node-character_sheet-form').once(function (index) {
      // Add the display to the form.
      vaxia_characters_init();
      // Now that it's added, add the click responses.
      $('.vaxia-character-sheet-templates a.vaxia-character-sheet-template-button').click(function() {
        vaxia_characters_template_fill(this);
        return false;
      });
    });

  });

  })(jQuery); }
}
