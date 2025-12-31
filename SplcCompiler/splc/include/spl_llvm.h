; basic LLVM functions used by SPL

declare i32 @printf ( i8*, ... )

declare i32 @scanf ( i8*, ... )

declare void @exit ( i32 )

@.sfi = private unnamed_addr constant [3 x i8] c"%i\00", align 1
@.sff = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@.sfs = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @puti ( i32 ) {
  %f = getelementptr inbounds [3 x i8], [3 x i8]* @.sfi, i32 0, i32 0
  call i32 (i8*, ...) @printf(i8* %f, i32 %0)
  ret i32 0
}

define i32 @geti ( i32* ) {
  %f = getelementptr inbounds [3 x i8], [3 x i8]* @.sfi, i32 0, i32 0
  call i32 (i8*, ...) @scanf(i8* %f, i32* %0)
  ret i32 0
}

define i32 @putf ( float ) {
  %f = getelementptr inbounds [3 x i8], [3 x i8]* @.sff, i32 0, i32 0
  %d = fpext float %0 to double
  call i32 (i8*, ...) @printf(i8* %f, double %d)
  ret i32 0
}

define i32 @getf ( float* ) {
  %f = getelementptr inbounds [3 x i8], [3 x i8]* @.sff, i32 0, i32 0
  call i32 (i8*, ...) @scanf(i8* %f, float* %0)
  ret i32 0
}

define i32 @gets ( i8* ) {
  %f = getelementptr inbounds [3 x i8], [3 x i8]* @.sfs, i32 0, i32 0
  call i32 (i8*, ...) @scanf(i8* %f, i8* %0)
  ret i32 0
}

@.new_line = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
