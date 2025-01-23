// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package test;

import store.Tool;
import store.Plant;
import store.Exposure;
import store.Item;
import store.Customer;



public class TestProduct {
    public static void main(String[] args) {
        int errors = 0;

        // Test #1: Verifying stock number
        Tool product1 = new Tool("Product1", 9);
        if (product1.getStockNumber() != 0) {
            System.err.println("Error Found: Stock number invalid " + product1.getStockNumber() + ", 0 is expected");
            errors++;
        }

        Tool product2 = new Tool("Product2", 10);
        if (product2.getStockNumber() != 1) {
            System.err.println("Error Found: Stock number invalid " + product2.getStockNumber() + ", 1 is expected");
            errors++;
        }

        // Test #2: Verifying getPrice
        if (product1.getPrice() != 9.99) {
            System.err.println("Error Found: Incorrect Price returned.");
            errors++;
        }

        // Test #3: Verifying toString Method
        if (!product1.toString().equals("Product1                       $ 9.99")) {
            System.err.println("ERROR: Incorrect representation for Product1");
            errors++;
        }

        // Test #4: Verifying Illegal Arguments for price (negative price)
        try {
            Tool product3 = new Tool("Product3", -1);
            System.err.println("Error Found: Invalid Price");
            errors++;
        } catch (IllegalArgumentException e) {
            // expected behavior
        } catch (Exception e) {
            System.err.println("ERROR: Unexpected exception occurred");
            errors++;
        }

        System.exit(errors);
    }
}

