public class TestProduct {
    public static void main(String[] args) {
        int errors = 0;

        // Test #1: Verifying stock number
        Product product1 = new Product("Product1", 9.99);
        if (product1.getStockNumber() != 0) {
            System.err.println("Error Found: Stock number invalid " + product1.getStockNumber() + ", 0 is expected");
            errors++;
        }

        Product product2 = new Product("Product2", 10.99);
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
            Product product3 = new Product("Product3", -1.99);
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

