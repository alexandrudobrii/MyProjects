package com.application.trafficsignrecognition;

import org.springframework.http.ResponseEntity;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.ServerConnection;
import com.application.utils.User;

/**
 * Activity which displays a login screen to the user, offering registration as
 * well.
 */
public class SignupActivity extends Activity {
	/**
	 * The default email to populate the email field with.
	 */
	public static final String EXTRA_EMAIL = "com.example.android.authenticatordemo.extra.EMAIL";
	private SharedPreferences       preferences;

	/**
	 * Keep track of the login task to ensure we can cancel it if requested.
	 */
	private UserLoginTask mAuthTask = null;
	private ServerConnection serverConnection = null;

	// Values for email and password at the time of the login attempt.
	private String mEmail;
	private String mPassword;
	private String mConfirmPassword;
	
	// UI references.
	private EditText mEmailView;
	private EditText mPasswordView;
	private EditText mConfirmPasswordView;
	private TextView mLogoView;
	private View mLoginFormView;
	private View mLoginStatusView;
	private TextView mLoginStatusMessageView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_signup);
		getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);

		preferences = getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		AndroidUtils.setRotation(this, preferences.getBoolean(Constants.ROTATE_SCREEN, false));
		
		// Establish connection with server
		serverConnection = new ServerConnection(preferences);
		
		// Set up the sign up form.
		mEmail = getIntent().getStringExtra(EXTRA_EMAIL);
		mEmailView = (EditText) findViewById(R.id.email_signup);
	
		mPasswordView = (EditText) findViewById(R.id.password_signup);
		mPasswordView
				.setOnEditorActionListener(new TextView.OnEditorActionListener() {
					public boolean onEditorAction(TextView textView, int id,
							KeyEvent keyEvent) {
						if (id == R.id.login || id == EditorInfo.IME_NULL) {
							attemptLogin();
							return true;
						}
						return false;
					}
				});
		
		mConfirmPasswordView = (EditText) findViewById(R.id.password_signup_confirm);
		mConfirmPasswordView
				.setOnEditorActionListener(new TextView.OnEditorActionListener() {
					public boolean onEditorAction(TextView textView, int id,
							KeyEvent keyEvent) {
						if (id == R.id.login || id == EditorInfo.IME_NULL) {
							attemptLogin();
							return true;
						}
						return false;
					}
				});

		mEmailView.setText("test@gmail.com");
		mPasswordView.setText("1234");
		mConfirmPasswordView.setText("1234");
		
		mLogoView = (TextView) findViewById(R.id.app_logo_sign_up);
		AndroidUtils.setFont(this, mLogoView, Constants.TEXT_FONT, Constants.APP_NAME);
		
		mLoginFormView = findViewById(R.id.signup_form);
		mLoginStatusView = findViewById(R.id.signup_status);
		mLoginStatusMessageView = (TextView) findViewById(R.id.signup_status_message);

		findViewById(R.id.sign_up_button).setOnClickListener(
				new View.OnClickListener() {
					public void onClick(View view) {
						attemptLogin();
					}
				});
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		getMenuInflater().inflate(R.menu.login, menu);
		return true;
	}

	/**
	 * Attempts to sign in or register the account specified by the login form.
	 * If there are form errors (invalid email, missing fields, etc.), the
	 * errors are presented and no actual login attempt is made.
	 */
	public void attemptLogin() {
		if (AndroidUtils.isInternetAvailable(this) == false) {
			AndroidUtils.showToast(this, "No internet connection available", Toast.LENGTH_SHORT);
			return;
		}
		
		if (mAuthTask != null) {
			return;
		}

		// Reset errors.
		mEmailView.setError(null);
		mPasswordView.setError(null);
		mConfirmPasswordView.setError(null);

		// Store values at the time of the login attempt.
		mEmail = mEmailView.getText().toString();
		mPassword = mPasswordView.getText().toString();
		mConfirmPassword = mConfirmPasswordView.getText().toString();
		
		boolean cancel = false;
		View focusView = null;

		// Check for a valid password.
		if (TextUtils.isEmpty(mPassword)) {
			mPasswordView.setError(getString(R.string.error_field_required));
			focusView = mPasswordView;
			cancel = true;
		} else if (mPassword.length() < 4) {
			mPasswordView.setError(getString(R.string.error_invalid_password));
			focusView = mPasswordView;
			cancel = true;
		}
		
		if (TextUtils.isEmpty(mConfirmPassword)) {
			mConfirmPasswordView.setError(getString(R.string.error_field_required));
			focusView = mConfirmPasswordView;
			cancel = true;
		}  else if (mConfirmPassword.length() < 4) {
			mConfirmPasswordView.setError(getString(R.string.error_invalid_password));
			focusView = mPasswordView;
			cancel = true;
		}
		
		if (cancel == false && !mPassword.equals(mConfirmPassword)) {
			mPasswordView.setError(getString(R.string.error_password_doesnt_match));
			mConfirmPasswordView.setError(getString(R.string.error_password_doesnt_match));
			focusView = mPasswordView;
			cancel = true;
		}
		
		// Check for a valid email address.
		if (TextUtils.isEmpty(mEmail)) {
			mEmailView.setError(getString(R.string.error_field_required));
			focusView = mEmailView;
			cancel = true;
		} else if (!mEmail.contains("@")) {
			mEmailView.setError(getString(R.string.error_invalid_email));
			focusView = mEmailView;
			cancel = true;
		}

		if (cancel) {
			// There was an error; don't attempt login and focus the first
			// form field with an error.
			focusView.requestFocus();
		} else {
			// Show a progress spinner, and kick off a background task to
			// perform the user login attempt.
			mLoginStatusMessageView.setText(R.string.login_progress_signing_up);
			showProgress(true);
			mAuthTask = new UserLoginTask();
			mAuthTask.execute((Void) null);
		}
	}

	/**
	 * Shows the progress UI and hides the login form.
	 */
	@TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
	private void showProgress(final boolean show) {
		// On Honeycomb MR2 we have the ViewPropertyAnimator APIs, which allow
		// for very easy animations. If available, use these APIs to fade-in
		// the progress spinner.
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB_MR2) {
			int shortAnimTime = getResources().getInteger(
					android.R.integer.config_shortAnimTime);

			mLoginStatusView.setVisibility(View.VISIBLE);
			mLoginStatusView.animate().setDuration(shortAnimTime)
					.alpha(show ? 1 : 0)
					.setListener(new AnimatorListenerAdapter() {
						@Override
						public void onAnimationEnd(Animator animation) {
							mLoginStatusView.setVisibility(show ? View.VISIBLE
									: View.GONE);
						}
					});

			mLoginFormView.setVisibility(View.VISIBLE);
			mLoginFormView.animate().setDuration(shortAnimTime)
					.alpha(show ? 0 : 1)
					.setListener(new AnimatorListenerAdapter() {
						@Override
						public void onAnimationEnd(Animator animation) {
							mLoginFormView.setVisibility(show ? View.GONE
									: View.VISIBLE);
						}
					});
		} else {
			// The ViewPropertyAnimator APIs are not available, so simply show
			// and hide the relevant UI components.
			mLoginStatusView.setVisibility(show ? View.VISIBLE : View.GONE);
			mLoginFormView.setVisibility(show ? View.GONE : View.VISIBLE);
		}
	}

	/**
	 * Represents an asynchronous login/registration task used to authenticate
	 * the user.
	 */
	public class UserLoginTask extends AsyncTask<Void, Void, Boolean> {
		@Override
		protected Boolean doInBackground(Void... params) {
			String url = serverConnection.getURI() + "user-management/sign_up";
			User user = new User(mEmail, mPassword);
			
			ResponseEntity<Boolean> response = 
					serverConnection.getConnection().postForEntity(url, user, Boolean.class);
			
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				return false;
			}
		
			return response.getBody();
		}

		@Override
		protected void onPostExecute(final Boolean success) {
			mAuthTask = null;
			showProgress(false);

			if (success) {
				Intent intent = new Intent(SignupActivity.this, MainMenuActivity.class);
				intent.putExtra(Constants.USERNAME, mEmail);
				startActivity(intent);
			} else {
				mEmailView
					.setError(getString(R.string.error_email_exist));
				mEmailView.requestFocus();
			}
		}

		@Override
		protected void onCancelled() {
			mAuthTask = null;
			showProgress(false);
		}
	}
}