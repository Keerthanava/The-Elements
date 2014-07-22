package com.idkjava.thelements;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class Register extends Activity{
	
	// This is the screen for registering the user
	
	Button SaveDetails;
	EditText Username;
	EditText Password;
	EditText Phone;
	EditText Email;
	
	SharedPreferences sharedpreferences;
	 public static final String PREFS_NAME = "RegisterUser" ;
	
	@Override
	   protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	      setContentView(R.layout.register_user);
	      
	        Username = (EditText)findViewById(R.id.edt_username);
		    Password = (EditText)findViewById(R.id.edt_password);
		    Phone = (EditText)findViewById(R.id.edt_email);
		    Email = (EditText)findViewById(R.id.edt_phone);
	      
	      SaveDetails= (Button) findViewById(R.id.SaveDetails);
	      
	}
	
	public void SaveDetails(View view){
		
		sharedpreferences = getSharedPreferences(PREFS_NAME,Context.MODE_PRIVATE);
		//sharedpreferences = PreferenceManager.getDefaultSharedPreferences(this);
		SharedPreferences.Editor editor = sharedpreferences.edit();
		
		
		editor.putString("Username", Username.getText().toString());
	    editor.putString("Password", Password.getText().toString());
	    editor.putString("Phone", Phone.getText().toString());
	    editor.putString("Email", Email.getText().toString());
	    
	    
	    editor.commit();
	    Toast.makeText(getApplicationContext(), "You have Registered successfully.Enter details to Login...", Toast.LENGTH_LONG).show();
	    //editor.clear();
	    Intent intent = new Intent(Register.this, Login.class);
	    startActivity(intent);
	}
	
	
	
	
	
	

}
