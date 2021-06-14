function Image = paintObject(Vertices, F, Colors, Depths, painter_style)
% Image = PAINTOBJECT(Vertices, F, Colors, Depths, painter_style)
%
% Vertices is a matrix with 2 columns and L rows which contains the
% coordinates of each vertex
% F is a Kx3 matrix: each row has the IDs of the 3 vertices of a triangle
% Colors is a Lx3 matrix: each row has the color of the corresponding vertex.
% Depths is a vertical Lx1 vector which contains the depth of each vertex
% (smaller depth means the vertex is closer to us)
% painter_style is either 'flat' or 'gouraud', depending on the method to use
% Image: is a MxNx3 matrix, the output of the function, the rendered object

  % constants
  M = 1200; % Diastaseis eikonas, dinontai apo ekfonisi
  N = 1200;
  num_Triangles = size(F, 1);
  %num_Vertices  = size(Vertices, 1);
  if (strcmpi(painter_style, 'gouraud')) % case insensitive comparison
    triPaint = @triPaintGouraud; % triangle painter function
  else
    triPaint = @triPaintFlat;
  end
  
  % initialise canvas with white background
  Image = ones(M, N, 3);
  
  
  %% Find the avg depth of each triangle
  DepthsTriangles = [zeros(num_Triangles, 1) transpose(1:num_Triangles)];
  % 1st column: avg depth, 2nd column: triangle id
  
  for i = 1:num_Triangles
    % F(i, :) is a vector with the ids of the 3 vertices-koryfes of triangle i
    % Depths( F(i, :) ) is a vector with the depths of the 3 vertices
    DepthsTriangles(i, 1) = mean( Depths( F(i, :) ));
  end
  
  DepthsTriangles = sortrows(DepthsTriangles, -1); % sort triangles by decreasing depth
  
  
  %% Paint the triangles
  
  for i = 1:num_Triangles
   % fprintf('%d / %d\n', i, num_Triangles);
    curr_triangle_id = DepthsTriangles(i, 2);
    curr_vertices_ids= F(curr_triangle_id, :);
    
    Image = triPaint(Image, Vertices(curr_vertices_ids, :), Colors(curr_vertices_ids, :));
    % paint vertices red
    % ImageN(Vertices(curr_vertices_ids,2), Vertices(curr_vertices_ids,1), 2:3)=0;
%     image(Image); %xlim([400 1000]);ylim([400 700]);
    
  end
  
end
