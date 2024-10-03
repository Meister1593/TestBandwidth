package com.plyshka.testbandwidth

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.plyshka.testbandwidth.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = "Library starting"
        binding.sampleText.text = meow()
    }

    /**
     * A native method that is implemented by the 'testbandwidth' native library,
     * which is packaged with this application.
     */
    external fun meow(): String;

    companion object {
        // Used to load the 'testbandwidth' library on application startup.
        init {
            System.loadLibrary("testbandwidth")
        }
    }
}