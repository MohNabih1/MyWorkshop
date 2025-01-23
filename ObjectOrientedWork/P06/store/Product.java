// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package store;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public abstract class Product {
    public Product(String name, int price) {
        if(price < 0) 
            throw new IllegalArgumentException("Invalid price of " + name + ": " + price/100 + "." + price%100);
        this.name = name;
        this.stockNumber = nextStockNumber++;
        this.price = price;
    }

public Product(BufferedReader reader) throws IOException {
        this.name = reader.readLine();
        this.price = Integer.parseInt(reader.readLine());
        this.stockNumber = Integer.parseInt(reader.readLine());
        Product.nextStockNumber = Integer.parseInt(reader.readLine());
    }

    public void save(BufferedWriter writer) throws IOException {
        writer.write(name);
        writer.newLine();
        writer.write(Integer.toString(price));
        writer.newLine();
        writer.write(Integer.toString(stockNumber));
        writer.newLine();
        writer.write(Integer.toString(nextStockNumber));
        writer.newLine();
    }

    public int getStockNumber() {
        return stockNumber;
    }
    public int getPrice() {
        return price;
    }
    @Override
    public String toString() {
        return String.format("%-40s $%5d.%02d", name, price/100, price%100);
    }
    private String name;
    private int stockNumber;
    private int price;
    private static int nextStockNumber = 0;
}
