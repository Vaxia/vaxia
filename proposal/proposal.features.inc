<?php
/**
 * @file
 * proposal.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function proposal_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function proposal_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function proposal_node_info() {
  $items = array(
    'proposal' => array(
      'name' => t('Proposal'),
      'base' => 'node_content',
      'description' => t('The proposal system allows you to build a form out of a collection of form pieces to put questions to site votes.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => t('<blockquote><i>"It is wrong to assume that Vaxia is the product of one person, or even one group of people. In different degrees, every single Vaxian is responsible for Vaxia; its creator and owner at once."</i><a href="http://vaxia.org/wiki/about-vaxia"><b> - SH Moon</b></a></blockquote>
We use proposals because while a forum thread may give you a rough feel for the mood of the site on a topic, it doesn\'t account for players who - for one reason or another - do not speak out on a forum thread. Silence does not at all indicate agreement, so we need to explicitly ask for a site vote when the topic is important, otherwise we risk leaving people out of getting a chance to contribute their opinions.
</br></br>
If you haven\'t looked over our recommendations for <a href="http://vaxia.org/wiki/help/i-have-seen-site-proposals-how-can-i-make-one-my-own">how to start a proposal you can find that in the wiki.</a> There\'s some good information in there about how to make sure your proposal takes into account multiple view points and recommended preparation for it, so if this is your first one you should start there.
</br></br>
The form will be open for two weeks, then it will automatically close. If you edit it in that timespan the timer will re-set. This is to make sure everyone can get a chance to weigh in, or update their vote if they change their mind.
</br></br>
Not all site members will be able to vote in this proposal - only those currently active when it was created. This is to avoid ballot stuffing or swoop-ins from long absent players.
</br></br>
And lastly, no one can see the names or IP addresses of the people who do vote. This is so they feel safe in making their wishes known. 
</br></br>
Once the proposal closes, the results will be posted publicly for all to see. Names will still anonymous.
</br></br>
Creating a proposal is a TWO-STEP process. Fill out the information here, and on saving it - the system will take you to part two to set up your form.'),
    ),
  );
  return $items;
}
