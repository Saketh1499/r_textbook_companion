<?php
// $Id$
function list_chapters()
  {
    global $user;
    /************************ start approve book details ************************/
    /*$proposal_q = db_query("SELECT * FROM {textbook_companion_proposal} WHERE uid = %d ORDER BY id DESC LIMIT 1", $user->uid);
    $proposal_data = db_fetch_object($proposal_q);*/
    $query = db_select('textbook_companion_proposal');
    $query->fields('textbook_companion_proposal');
    $query->condition('uid', $user->uid);
    $query->orderBy('id', 'DESC');
    $query->range(0, 1);
    $result = $query->execute();
    $proposal_data = $result->fetchObject();
    if (!$proposal_data)
      {
        drupal_set_message("Please submit a " . l('proposal', 'textbook-companion/proposal') . ".", 'error');
        drupal_goto('');
      }
    if ($proposal_data->proposal_status != 1 && $proposal_data->proposal_status != 4)
      {
        switch ($proposal_data->proposal_status)
        {
            case 0:
                drupal_set_message(t('We have already received your proposal. We will get back to you soon.'), 'status');
                drupal_goto('');
                return;
                break;
            case 2:
                drupal_set_message(t('Your proposal has been dis-approved. Please create another proposal ' . l('here', 'textbook-companion/proposal') . '.'), 'error');
                drupal_goto('');
                return;
                break;
            case 3:
                drupal_set_message(t('Congratulations! You have completed your last book proposal. You have to create another proposal ' . l('here', 'textbook-companion/proposal') . '.'), 'status');
                drupal_goto('');
                return;
                break;
            default:
                drupal_set_message(t('Invalid proposal state. Please contact site administrator for further information.'), 'error');
                drupal_goto('');
                return;
                break;
        }
      }
      
      
    /*$preference_q = db_query("SELECT * FROM {textbook_companion_preference} WHERE proposal_id = %d AND approval_status = 1 LIMIT 1", $proposal_data->id);
    $preference_data = db_fetch_object($preference_q);*/
    $query = db_select('textbook_companion_preference');
    $query->fields('textbook_companion_preference');
    $query->condition('proposal_id', $proposal_data->id);
    $query->condition('approval_status', 1);
    $query->range(0, 1);
    $result = $query->execute();
    $preference_data = $result->fetchObject();
    if ($preference_data->submited_all_examples_code == 1)
      {
      drupal_set_message(t('You have submited your all codes for this book to review, hence you can not upload more code, for any query please write us.'), 'status');
                drupal_goto('');
                return;       
      }
    if (!$preference_data)
      {
        drupal_set_message(t('Invalid Book Preference status. Please contact site administrator for further information.'), 'error');
        drupal_goto('');
        return;
      }
    /************************ end approve book details **************************/
    $return_html = '<br />';
    $return_html .= '<strong>Title of the Book:</strong><br />' . $preference_data->book . '<br /><br />';
    $return_html .= '<strong>Contributor Name:</strong><br />' . $proposal_data->full_name . '<br /><br />';
    $return_html .= l('Upload Example Code', 'textbook-companion/code/upload') . '<br />';
    /* get chapter list */
    $chapter_rows = array();
    /*$chapter_q = db_query("SELECT * FROM {textbook_companion_chapter} WHERE preference_id = %d ORDER BY number ASC", $preference_data->id);*/
    $query = db_select('textbook_companion_chapter');
    $query->fields('textbook_companion_chapter');
    $query->condition('preference_id', $preference_data->id);
    $query->orderBy('number', 'ASC');
    $chapter_q = $query->execute();
    while ($chapter_data = $chapter_q->fetchObject())
      {
        /* get example list */
        /* $example_q = db_query("SELECT count(*) as example_count FROM {textbook_companion_example} WHERE chapter_id = %d", $chapter_data->id);
        $example_data = db_fetch_object($example_q);*/
        $query = db_select('textbook_companion_example');
        $query->addExpression('count(*)', 'example_count');
        $query->condition('chapter_id', $chapter_data->id);
        $result = $query->execute();
        $example_data = $result->fetchObject();
        $chapter_rows[] = array(
            $chapter_data->number,
            $chapter_data->name . ' (' . l('Edit', 'textbook-companion/code/chapter/edit/' . $chapter_data->id) . ')',
            $example_data->example_count,
            l('View', 'textbook-companion/code/list-examples/' . $chapter_data->id)
        );
      }
    /* check if there are any chapters */
    if (!$chapter_rows)
      {
        drupal_set_message(t('No uploads found.'), 'status');
        return $return_html;
      }
    $chapter_header = array(
        'Chapter No.',
        'Title of the Chapter',
        'Uploaded Examples',
        'Actions'
    );
    $return_html .= theme('table', array(
        'header' => $chapter_header,
        'rows' => $chapter_rows
    ));
     $submited_all_example = drupal_get_form("all_example_submitted_check_form",$preference_data->id);
    $return_html .= drupal_render($submited_all_example);
    return $return_html;
  }
function list_examples()
  {
    global $user;
    /************************ start approve book details ************************/
    /*$proposal_q = db_query("SELECT * FROM {textbook_companion_proposal} WHERE uid = %d ORDER BY id DESC LIMIT 1", $user->uid);
    $proposal_data = db_fetch_object($proposal_q);*/
    $query = db_select('textbook_companion_proposal');
    $query->fields('textbook_companion_proposal');
    $query->condition('uid', $user->uid);
    $query->orderBy('id', 'DESC');
    $query->range(0, 1);
    $result = $query->execute();
    $proposal_data = $result->fetchObject();
    if (!$proposal_data)
      {
        drupal_set_message("Please submit a " . l('proposal', 'textbook-companion/proposal') . ".", 'error');
        drupal_goto('');
      }
    if ($proposal_data->proposal_status != 1 && $proposal_data->proposal_status != 4)
      {
        switch ($proposal_data->proposal_status)
        {
            case 0:
                drupal_set_message(t('We have already received your proposal. We will get back to you soon.'), 'status');
                drupal_goto('');
                return;
                break;
            case 2:
                drupal_set_message(t('Your proposal has been dis-approved. Please create another proposal ' . l('here', 'textbook-companion/proposal') . '.'), 'error');
                drupal_goto('');
                return;
                break;
            case 3:
                drupal_set_message(t('Congratulations! You have completed your last book proposal. You have to create another proposal ' . l('here', 'textbook-companion/proposal') . '.'), 'status');
                drupal_goto('');
                return;
                break;
            default:
                drupal_set_message(t('Invalid proposal state. Please contact site administrator for further information.'), 'error');
                drupal_goto('');
                return;
                break;
        }
      }
    /*$preference_q = db_query("SELECT * FROM {textbook_companion_preference} WHERE proposal_id = %d AND approval_status = 1 LIMIT 1", $proposal_data->id);
    $preference_data = db_fetch_object($preference_q);*/
    $query = db_select('textbook_companion_preference');
    $query->fields('textbook_companion_preference');
    $query->condition('proposal_id', $proposal_data->id);
    $query->condition('approval_status', 1);
    $query->range(0, 1);
    $result = $query->execute();
    $preference_data = $result->fetchObject();
    if (!$preference_data)
      {
        drupal_set_message(t('Invalid Book Preference status. Please contact site administrator for further information.'), 'error');
        drupal_goto('');
        return;
      }
    /************************ end approve book details **************************/
    /* get chapter details */
    $chapter_id = arg(3);
    /*$chapter_q = db_query("SELECT * FROM {textbook_companion_chapter} WHERE id = %d AND preference_id = %d LIMIT 1", $chapter_id, $preference_data->id);*/
    $query = db_select('textbook_companion_chapter');
    $query->fields('textbook_companion_chapter');
    $query->condition('id', $chapter_id);
    $query->condition('preference_id', $preference_data->id);
    $query->range(0, 1);
    $chapter_q = $query->execute();
    if ($chapter_data = $chapter_q->fetchObject())
      {
        $return_html = '<br />';
        $return_html .= '<strong>Title of the Book:</strong><br />' . $preference_data->book . '<br /><br />';
        $return_html .= '<strong>Contributor Name:</strong><br />' . $proposal_data->full_name . '<br /><br />';
        $return_html .= '<strong>Chapter Number:</strong><br />' . $chapter_data->number . '<br /><br />';
        $return_html .= '<strong>Title of the Chapter:</strong><br />' . $chapter_data->name . '<br />';
      }
    else
      {
        drupal_set_message(t('Invalid chapter.'), 'error');
        drupal_goto('textbook-companion/code');
        return;
      }
    $return_html .= '<br />' . l('Back to Chapter List', 'textbook-companion/code');
    /* get example list */
    $example_rows = array();
    $query = db_select('textbook_companion_example');
    $query->fields('textbook_companion_example');
    $query->condition('chapter_id', $chapter_id);
    $example_q = $query->execute();
    while ($example_data = $example_q->fetchObject())
      {
        /* approval status */
        $approval_status = '';
        switch ($example_data->approval_status)
        {
            case 0:
                $approval_status = 'Pending';
                break;
            case 1:
                $approval_status = 'Approved';
                break;
            case 2:
                $approval_status = 'Rejected';
                break;
        }
        /* example files */
        $example_files = '';
        /*$example_files_q = db_query("SELECT * FROM {textbook_companion_example_files} WHERE example_id = %d ORDER BY filetype", $example_data->id);*/
        $query = db_select('textbook_companion_example_files');
        $query->fields('textbook_companion_example_files');
        $query->condition('example_id', $example_data->id);
        $query->orderBy('filetype', 'ASC');
        $example_files_q = $query->execute();
        while ($example_files_data = $example_files_q->fetchObject())
          {
            $file_type = '';
            switch ($example_files_data->filetype)
            {
                case 'S':
                    $file_type = 'Main or Source';
                    break;
                case 'D':
                    $file_type = 'Dataset';
                    break;
                case 'X':
                    $file_type = 'xcos';
                    break;
                default:
            }
            $example_files .= l($example_files_data->filename, 'textbook-companion/download/file/' . $example_files_data->id) . ' (' . $file_type . ')<br />';
          }
        if ($example_data->approval_status == 0)
          {
            $example_rows[] = array(
                'data' => array(
                    $example_data->number,
                    $example_data->caption,
                    $approval_status,
                    $example_files,
                    l('Edit', 'textbook-companion/code/edit/' . $example_data->id) . ' | ' . l('Delete', 'textbook-companion/code/delete/' . $example_data->id, array(
                        'attributes' => array(
                            'onClick' => 'return confirm("Are you sure you want to delete the example?")'
                        )
                    ))
                ),
                'valign' => 'top'
            );
          }
        else
          {
            $example_rows[] = array(
                'data' => array(
                    $example_data->number,
                    $example_data->caption,
                    $approval_status,
                    $example_files,
                    l('Download', 'textbook-companion/download/example/' . $example_data->id)
                ),
                'valign' => 'top'
            );
          }
      }
    $example_header = array(
        'Example No.',
        'Caption',
        'Status',
        'Files',
        'Action'
    );
    $return_html .= theme('table', array(
        'header' => $example_header,
        'rows' => $example_rows
    ));
   
    return $return_html;
  }
function all_example_submitted_check_form($form,&$form_state,$preference_id){
    //$form = array();
    $query = db_select('textbook_companion_preference');
    $query->fields('textbook_companion_preference');
    $query->condition('id', $preference_id);
    $query->condition('approval_status', 1);
    $query->range(0, 1);
    $result = $query->execute();
    $preference_data = $result->fetchObject();
    $form['#attributes'] = array(
        'enctype' => "multipart/form-data"
    );
    $form['all_example_submitted'] = array(
            '#type' => 'checkbox',
            '#title' => t('I have submitted codes for all the examples'),
            '#description' => 'Once you have submited this option you are not able to upload more examples.',
            '#required' => TRUE,
    );
    //var_dump($preference_data->approved_codable_example_files);die;
    if($preference_data->approved_codable_example_files == 0){
    $form['upload_codable_examples'] = array(
        '#type' => 'file',
        '#states' => array(
            'visible' => array(
                ':input[name="all_example_submitted"]' => array(
                    'checked' => True
                )
            )
        ),
        '#title' => t('Upload a document file containing a list of codable examples as shown in the template <a href="https://static.fossee.in/r/manuals/Template.docx" target="_blank">here</a><span style="color:red">*</span>'),
        '#description' => t('Separate filenames with underscore. No spaces or any special characters allowed in filename.') . '<br />' . t('<span style="color:red;">Allowed file extensions : ') . variable_get('textbook_companion_codable_examples_extensions', '') . '</span>',
        );
    }
        $form['hidden_preference_id'] = array(
        '#type' => 'hidden',
        '#value' => $preference_id
    );
        $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Submit')
    );
        return $form;
}
function all_example_submitted_check_form_validate($form,&$form_state){
if ($form_state['values']['all_example_submitted'] != 1) {
    form_set_error('all_example_submitted', t('Please check the field if you are intrested to submit the all uploaded examples for review!'));
  }
if (isset($_FILES['files']))
    {
        /* check if atleast one source or result file is uploaded */
        if (!($_FILES['files']['name']['upload_codable_examples']))
            form_set_error('upload_codable_examples', t('Please upload the file containing a list of codable examples.'));
        /* check for valid filename extensions */
        foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
        {
            if ($file_name)
            {
                /* checking file type */
                $allowed_extensions_str = variable_get('textbook_companion_codable_examples_extensions', '');
                $allowed_extensions = explode(',', $allowed_extensions_str);
                $fnames = explode('.', strtolower($_FILES['files']['name'][$file_form_name]));
                $temp_extension = end($fnames);
                if (!in_array($temp_extension, $allowed_extensions))
                    form_set_error($file_form_name, t('Only file with ' . $allowed_extensions_str . ' extensions can be uploaded.'));
                if ($_FILES['files']['size'][$file_form_name] <= 0)
                    form_set_error($file_form_name, t('File size cannot be zero.'));
                /* check if valid file name */
                if (!textbook_companion_check_valid_filename($_FILES['files']['name'][$file_form_name]))
                    form_set_error($file_form_name, t('Invalid file name specified. Only alphabets and numbers are allowed as a valid filename.'));
            } //$file_name
        } //$_FILES['files']['name'] as $file_form_name => $file_name
    } 
  return;
}

function all_example_submitted_check_form_submit($form,&$form_state){
global $user;
/*if ($form_state['values']['all_example_submitted'] == 1) {
db_query('UPDATE textbook_companion_preference SET submited_all_examples_code = 1 WHERE id = :preference_id',array(':preference_id' => $form_state['values']['hidden_preference_id']));*/
$query = db_select('textbook_companion_proposal');
    $query->fields('textbook_companion_proposal');
    $query->condition('uid', $user->uid);
    $query->orderBy('id', 'DESC');
    $query->range(0, 1);
    $result = $query->execute();
    $proposal_data = $result->fetchObject();
    if (!$proposal_data)
      {
        drupal_set_message("Please submit a " . l('proposal', 'textbook-companion/proposal') . ".", 'error');
        drupal_goto('');
      }
    if ($proposal_data->proposal_status != 1 && $proposal_data->proposal_status != 4)
      {
        switch ($proposal_data->proposal_status)
        {
            case 0:
                drupal_set_message(t('We have already received your proposal. We will get back to you soon.'), 'status');
                drupal_goto('');
                return;
                break;
            case 2:
                drupal_set_message(t('Your proposal has been dis-approved. Please create another proposal ' . l('here', 'textbook-companion/proposal') . '.'), 'error');
                drupal_goto('');
                return;
                break;
            case 3:
                drupal_set_message(t('Congratulations! You have completed your last book proposal. You have to create another proposal ' . l('here', 'textbook-companion/proposal') . '.'), 'status');
                drupal_goto('');
                return;
                break;
            case 5:
                drupal_set_message(t('You have submmited your all codes'), 'status');
                drupal_goto('');
                return;
                break;
            default:
                drupal_set_message(t('Invalid proposal state. Please contact site administrator for further information.'), 'error');
                drupal_goto('');
                return;
                break;
        }
      }
    $query = db_select('textbook_companion_preference');
    $query->fields('textbook_companion_preference');
    $query->condition('proposal_id', $proposal_data->id);
    $query->condition('approval_status', 1);
    $query->range(0, 1);
    $result = $query->execute();
    $preference_data = $result->fetchObject();
    if (!$preference_data)
      {
        drupal_set_message(t('Invalid Book Preference status. Please contact site administrator for further information.'), 'error');
        drupal_goto('');
        return;
      }
      if($preference_data->approved_codable_example_files == 0){
$root_path = textbook_companion_path();
$proposal_directory = $preference_data->directory_name;
    $dest_path = $proposal_directory . '/';
    if (!is_dir($root_path . $dest_path)){   
        if(!mkdir($root_path . $dest_path))
        {
        drupal_set_message(t('You cannot upload your code. Error in creating directory'), 'error');
        }
     }   
    /* creating directories */
    $dest_path .= 'codable_example_file';
    if (!is_dir($root_path . $dest_path))
        mkdir($root_path . $dest_path);
    //var_dump($root_path . $dest_path);die;
    $filepath = 'codable_example_file/';
    foreach ($_FILES['files']['name'] as $file_form_name => $file_name)
      {
                    if (file_exists($root_path . $dest_path . '/' . $_FILES['files']['name'][$file_form_name]))
                      {
                        move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . '/' . $_FILES['files']['name'][$file_form_name]);
                        drupal_set_message(t("File !filename already exists hence overwirtten the exisitng file ", array(
                            '!filename' => $_FILES['files']['name'][$file_form_name]
                        )), 'status');
                        $query = "UPDATE {textbook_companion_codable_example_files} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id";
                            $args = array(
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $filepath . $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path . '/' . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":timestamp" => time(),
                                ":proposal_id" => $preference_data->proposal_id
                            );
                            db_query($query, $args);
                        db_query('UPDATE textbook_companion_preference SET submited_all_examples_code = 1, submitted_codable_examples_file = 1 WHERE id = :preference_id',array(':preference_id' => $preference_data->id));
                        return;
                      }
                    /* uploading file */
                    else if (move_uploaded_file($_FILES['files']['tmp_name'][$file_form_name], $root_path . $dest_path . '/' . $_FILES['files']['name'][$file_form_name]))
                      {
                        $query_codable_examples_file = "SELECT * FROM textbook_companion_codable_example_files WHERE proposal_id = :proposal_id";
                        $args_query_codable_examples_file = array(
                            ":proposal_id" => $preference_data->proposal_id
                        );
                        $query_codable_examples_file_result = db_query($query_codable_examples_file, $args_query_codable_examples_file)->fetchObject();
                        if(!$query_codable_examples_file_result){
                        $query = "INSERT INTO {textbook_companion_codable_example_files} (proposal_id, filename, filepath,filemime, filesize, filetype, timestamp) VALUES (:proposal_id, :filename ,:filepath,:filemime, :filesize, :filetype, :timestamp)";
                        $args = array(
                                ":proposal_id" => $preference_data->proposal_id,
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $filepath . $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path . '/' . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":filetype" => 'C',
                                ":timestamp" => time()
                        );
                        $result = db_query($query, $args, array(
                            'return' => Database::RETURN_INSERT_ID
                        ));
                        drupal_set_message('File uploaded successfully.', 'status');
                      }
                      else
                      {
                            unlink($root_path . $dest_path . '/' . $query_codable_examples_file_result->filename);
                            $query = "UPDATE {textbook_companion_codable_example_files} SET filename = :filename, filepath=:filepath, filemime=:filemime, filesize=:filesize, timestamp=:timestamp WHERE proposal_id = :proposal_id";
                            $args = array(
                                ":filename" => $_FILES['files']['name'][$file_form_name],
                                ":filepath" => $filepath . $_FILES['files']['name'][$file_form_name],
                                ":filemime" => mime_content_type($root_path . $dest_path . '/' . $_FILES['files']['name'][$file_form_name]),
                                ":filesize" => $_FILES['files']['size'][$file_form_name],
                                ":timestamp" => time(),
                                ":proposal_id" => $preference_data->proposal_id
                            );
                            db_query($query, $args);
                            drupal_set_message($file_name . ' file updated successfully.', 'status');
                      }
                    db_query('UPDATE textbook_companion_preference SET submited_all_examples_code = 1, submitted_codable_examples_file = 1 WHERE id = :preference_id',array(':preference_id' => $preference_data->id));
                  }
                    else
                      {
                        drupal_set_message('Error uploading file : ' . $dest_path . '/' . $file_name, 'error');
                      }
          }  
      }
      else{
        db_query('UPDATE textbook_companion_preference SET submited_all_examples_code = 1 WHERE id = :preference_id',array(':preference_id' => $preference_data->id));
      }
    //drupal_set_message('File uploaded successfully.', 'status');
 /* sending email */
 $query = ("SELECT proposal_id FROM textbook_companion_preference WHERE id= :preference_id");
 $args = array(":preference_id" => $form_state['values']['hidden_preference_id']);
 $proposal_data = db_query($query,$args);
 $proposal_data_result = $proposal_data->fetchObject();

    $email_to = $user->mail;
    $from = variable_get('textbook_companion_from_email', '');
    $bcc = variable_get('textbook_companion_emails', '');
    $cc = variable_get('textbook_companion_cc_emails', '');
    $param['all_code_submitted']['proposal_id'] = $proposal_data_result->proposal_id;
    $param['all_code_submitted']['user_id'] = $user->uid;
    $param['all_code_submitted']['headers'] = array(
        'From' => $from,
        'MIME-Version' => '1.0',
        'Content-Type' => 'text/plain; charset=UTF-8; format=flowed; delsp=yes',
        'Content-Transfer-Encoding' => '8Bit',
        'X-Mailer' => 'Drupal',
        'Cc' => $cc,
        'Bcc' => $bcc
    );
    if (!drupal_mail('textbook_companion', 'all_code_submitted', $email_to, language_default(), $param, $from, TRUE))
        drupal_set_message('Error sending email message.', 'error');
}

