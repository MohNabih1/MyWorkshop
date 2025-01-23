// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
package store;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public class Item {
    public Item(Product product, int quantity) {
        this.product = product;
        this.quantity = quantity;
    }

public Item(BufferedReader reader) throws IOException {
        String productType = reader.readLine();
        this.product = productType.equals("store.Plant") ? new Plant(reader) : new Tool(reader);
        this.quantity = Integer.parseInt(reader.readLine());
    }

public void save(BufferedWriter writer) throws IOException {
        writer.write(product.getClass().getName());
        writer.newLine();
        product.save(writer);
        writer.write(Integer.toString(quantity));
        writer.newLine();
    }
    public int getPrice() {
        return product.getPrice() * quantity;
    }
    @Override
    public String toString() {
        int price = getPrice();
        return String.format("%3d %-50s $ %5d.%02d", quantity, product, price/100, price%100);
    }
    private Product product;
    private int quantity;
}
