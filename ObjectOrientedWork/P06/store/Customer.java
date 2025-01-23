// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package store;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public class Customer {
    public Customer(String name, String email) {
        int at = email.indexOf("@");
        int dot = email.indexOf(".", at+1);
        if(at < 0 || dot < 0) 
            throw new  IllegalArgumentException("Invalid email address: " + email);
        this.name = name;
        this.email = email;
    }

 // To read from the BufferReader
    public Customer(BufferedReader reader) throws IOException {
       
        this.name = reader.readLine();
        this.email = reader.readLine(); 
  }



// Saving the Customer info to BufferReader
    public void save(BufferedWriter writer) throws IOException {
        writer.write(name);
        writer.newLine(); 
        writer.write(email);
        writer.newLine(); 
  }


    @Override
    public String toString() {
        return String.format("%s (%s)", name, email);
    }
    private String name;
    private String email;
}
